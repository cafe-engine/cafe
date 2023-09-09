extern crate tea;

use tea::{GlUniform, GlBind};
use tea::texture::{Texture2D, GlTexture};
use crate::{Mat4, Vec4, Vec3, Rect};

use tea::buffer::{GlBuffer, BufferUsage};
use tea::target::{Framebuffer, FramebufferAttachment};

pub use tea::texture::{TexTarget, TexFilter, TexWrap, TexParam, PixelFormat, PixelDataType};

#[allow(non_snake_case)]
pub mod render2D;
#[allow(non_snake_case)]
pub mod render3D;

#[derive(Default, Debug, Copy, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub struct Color(u8, u8, u8, u8);

impl Color {
    pub fn rgb(r: u8, g: u8, b: u8) -> Color { Color(r, g, b, 255) }
    pub fn rgba(r: u8, g: u8, b: u8, a: u8) -> Color { Color(r, g, b, a) }

    pub fn black() -> Self { Self::rgb(0, 0, 0) }
    pub fn white() -> Self { Self::rgb(255, 255, 255) }

    pub fn to_vec4(&self) -> Vec4 {
        let r = self.0 as f32 / 255.0;
        let g = self.1 as f32 / 255.0;
        let b = self.2 as f32 / 255.0;
        let a = self.3 as f32 / 255.0;
        Vec4::new(r, g, b, a)
    }
}

pub enum Movement {
    Left,
    Right,
    Up,
    Down,
    Forward,
    Backward,
}

pub trait Camera {
    fn position(&self) -> &Vec3;
    fn set_position(&mut self, position: Vec3);

    fn set_width(&mut self, width: f32);
    fn set_height(&mut self, height: f32);
    fn set_size(&mut self, width: f32, height: f32) { self.set_width(width); self.set_height(height); }
    fn projection(&self) -> Mat4;
    fn view(&self) -> Mat4;

    fn move_camera(&mut self, dir: Movement, dt: f32);
}

pub trait Drawable {
    fn get_handle(&self) -> &Texture2D;
    fn get_width(&self) -> i32;
    fn get_height(&self) -> i32;
    fn flip_y(&self) -> bool { false }
}

pub trait Batch {
    type DataType;
    fn clear(&mut self);
    fn capacity(&self) -> usize;
    fn len(&self) -> usize;
    fn grow(&mut self);
    fn push(&mut self, value: Self::DataType);
    fn push_vec(&mut self, values: Vec<Self::DataType>);
    fn flush(&self);
    fn flush_part(&self, offset: isize, size: usize);
}

#[derive(Default, Debug, PartialEq, Eq)]
pub struct Image {
    texture: Texture2D,
    width: i32, height: i32,
    filter: (TexFilter, TexFilter),
    wrap: (TexWrap, TexWrap)
}

#[derive(Default, Debug, PartialEq, Eq)]
pub struct Canvas {
    framebuffer: Framebuffer,
    texture: Texture2D,
    width: i32, height: i32,
    filter: (TexFilter, TexFilter),
    wrap: (TexWrap, TexWrap),
}

#[derive(Default, Debug)]
pub struct Buffer<K: GlBuffer, T> {
    handle: K,
    data: Vec<T>
}

impl<K: GlBuffer, T> PartialEq for Buffer<K, T> {
    fn eq(&self, other: &Self) -> bool { self.handle.get_id() == other.handle.get_id() }
}

impl<K: GlBuffer, T> Batch for Buffer<K, T> {
    type DataType = T;
    fn clear(&mut self) { self.data.clear() }
    fn capacity(&self) -> usize { self.data.capacity() }
    fn len(&self) -> usize { self.data.len() }
    fn push(&mut self, value: T) {
        self.data.push(value);
    }

    fn push_vec(&mut self, values: Vec<T>) {
        self.data.extend(values);
    }

    fn grow(&mut self) {
        let count = self.data.capacity();
        let v: Vec<T> = Vec::with_capacity(count);
        let len = self.data.len();
        self.data.extend(v);
        unsafe { self.data.set_len(len); }
        self.handle.bind();
        self.handle.data(&self.data, BufferUsage::DynamicDraw);
    }

    fn flush(&self) {
        self.handle.bind();
        self.handle.subdata(0, self.data.len(), &self.data);
        self.handle.unbind();
    }

    fn flush_part(&self, offset: isize, count: usize) {
        self.handle.bind();
        self.handle.subdata(offset, count, &self.data);
        self.handle.unbind();
    }
}

#[cfg(target="")]
#[derive(Default, Debug)]
pub struct VertexBuffer<T> {
    handle: ArrayBuffer,
    data: Vec<T>
}

impl Image {
    fn new(width: i32, height: i32, format: PixelFormat, data: Vec<u8>) -> Result<Self, String> {
        let texture = Texture2D::new().unwrap();
        texture.bind();
        texture.data(0, format, (width, height), PixelDataType::UByte, data);
        texture.set_min_filter(TexFilter::default());
        texture.set_mag_filter(TexFilter::default());
        texture.unbind();
        Ok(Image{
            texture,
            width, height,
            ..Default::default()
        })
    }

    pub fn load(filename: &str) -> Result<Self, String> {
        let res = stb_image::image::load(filename);
        match res {
            stb_image::image::LoadResult::Error(s) => {
                let err = format!("Failed to load image: {filename}: {s}");
                return Err(err)
            },
            stb_image::image::LoadResult::ImageU8(i) => {
                let format: PixelFormat;
                match i.depth {
                    3 => { format = PixelFormat::RGB },
                    4 => { format = PixelFormat::RGBA },
                    _ => { format = PixelFormat::RGBA }
                }
                return Image::new(i.width as i32, i.height as i32, format, i.data);
            },
            _ => {}
        }
        Ok(Image::default())
    }

    pub fn pixel_art(&self) {
        self.texture.bind();
        self.texture.set_min_filter(TexFilter::Nearest);
        self.texture.set_mag_filter(TexFilter::Nearest);
        self.texture.unbind();
    }
    
    pub fn pixel(&self, x: i32, y: i32) -> Color {
        Color::rgb(255, 255, 255)
    }
}

impl Drawable for Image {
    fn get_handle(&self) -> &Texture2D { &self.texture }
    fn get_width(&self) -> i32 { self.width }
    fn get_height(&self) -> i32 { self.height }
}

impl Canvas {
    pub fn new(width: i32, height: i32) -> Result<Self, String> {
        let framebuffer = Framebuffer::new().unwrap();
        let texture = Texture2D::new().unwrap();
        texture.bind();
        texture.data(0, PixelFormat::RGBA, (width, height), PixelDataType::UByte, vec![]);
        texture.set_min_filter(TexFilter::Linear);
        texture.set_mag_filter(TexFilter::Linear);
        texture.unbind();
        framebuffer.bind();
        framebuffer.attach_texture(FramebufferAttachment::ColorAttachment(0), &texture);
        framebuffer.unbind();
        Ok(Canvas{
            framebuffer,
            texture,
            width,
            height,
            ..Default::default()
        })
    }

    pub fn pixel_art(&self) {
        self.texture.bind();
        self.texture.set_min_filter(TexFilter::Nearest);
        self.texture.set_mag_filter(TexFilter::Nearest);
        self.texture.unbind();
    }
}

impl Drawable for Canvas {
    fn get_handle(&self) -> &Texture2D { &self.texture }
    fn get_width(&self) -> i32 { self.width }
    fn get_height(&self) -> i32 { self.height }
    fn flip_y(&self) -> bool { true }
}

#[allow(dead_code)]
#[derive(Default, Debug, PartialEq)]
pub struct Font {
    texture: Texture2D,
    rect: Vec<Rect>,
    size: u16
}

impl Font {
    pub fn load(filename: &str) -> Result<Self, String> {
        Ok(Font::default())
    }
}

pub trait Effect {
   fn send_uniform<T: GlUniform>(&self, name: &str, value: T);
}

pub trait Render {
    type Effect: Effect;
    fn new() -> Self;

    fn clear_color(&self, color: Vec4) { tea::clear_color(color.x, color.y, color.z, color.w); }
    fn clear(&self) { tea::clear(&[tea::ClearFlags::ColorBufferBit]) }

    fn begin(&mut self);
    fn end(&self);

    fn camera(&mut self) -> &mut dyn Camera;

    fn set_canvas(&mut self, canvas: Option<&Canvas>);
    fn set_effect(&mut self, effect: Option<&Self::Effect>);

    fn on_resize(&mut self, width: i32, height: i32);
}
