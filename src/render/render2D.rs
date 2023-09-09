extern crate tea;
extern crate stb_image;

use crate::render::{Image, Canvas, Drawable, Batch, Buffer, Movement};

use tea::vertex::{VertexArray, VertexData, VertexFormatBuilder, VertexFormat};
use tea::{glsl::*, EnableFlags};
use tea::texture::{Texture2D, PixelFormat, Sampler2D};
use tea::target::Framebuffer;
use tea::impl_vertexdata;
use tea::vec::*;
use tea::{GlBind, GlUniform, GlObject, GlTarget, GlUse, DrawMode, BlendFunc};
use tea::buffer::{ArrayBuffer, ElementArrayBuffer, GlBuffer, BufferUsage};
use crate::render::{Effect, Render, Camera};
use crate::{Rect, Color};


impl_vertexdata!(Vertex2D, Vec2, Vec4, Vec2);

type Index = (i32, i32, i32, i32, i32, i32);

pub type VertexBuffer = Buffer<ArrayBuffer, Vertex2D>;
pub type IndexBuffer = Buffer<ElementArrayBuffer, Index>;

impl VertexBuffer {
    pub fn new(size: usize) -> Result<Self, String> {
        let handle = ArrayBuffer::new();
        let data: Vec<Vertex2D> = Vec::with_capacity(size);
        handle.bind();
        handle.data(&data, BufferUsage::DynamicDraw);
        handle.unbind();
        Ok(Self {
            handle,
            data
        })
    }
}

impl IndexBuffer {
    pub fn new(size: usize) -> Result<Self, String> {
        let handle = ElementArrayBuffer::new();
        let mut data: Vec<Index> = Vec::with_capacity(size);
        for i in 0..(size as i32) {
            let index = (i, i+1, i+2, i+3, i+1, i);
            data.push(index);
        }
        handle.bind();
        handle.data(&data, BufferUsage::StaticDraw);
        handle.unbind();
        Ok(Self {
            handle,
            data
        })
    }
}

#[derive(Default, Debug, PartialEq, Copy, Clone)]
pub struct Camera2D {
    position: Vec3,
    right: Vec3,
    width: f32, height: f32,
    near: f32, far: f32,
    move_speed: f32,

    zoom: f32
}

impl Camera for Camera2D {
    fn position(&self) -> &Vec3 { &self.position }
    fn set_position(&mut self, position: Vec3) { self.position = position; }

    fn set_width(&mut self, width: f32) { self.width = width; }
    fn set_height(&mut self, height: f32) { self.height = height; }

    fn projection(&self) -> Mat4 {
        Mat4::ortho(0.0, self.width, self.height, 0.0, self.near, self.far)
    }
    
    fn view(&self) -> Mat4 {
        Mat4::identity()
    }

    fn move_camera(&mut self, dir: Movement, dt: f32) {
        let velocity = self.move_speed * dt;
        match dir {
            Movement::Right => { self.position += self.right.scalar(velocity); },
            Movement::Left => { self.position -= self.right.scalar(velocity); },
            _ => {}
        }
    }
}

impl Camera2D {
    pub fn new(width: f32, height: f32) -> Self {
        let position = Vec3::zero();
        let right = Vec3::new(1.0, 0.0, 0.0);
        let near = -0.1;
        let far = 0.1;
        let move_speed = 60.0;
        let zoom = 1.0;

        Self {
            position,
            right,
            width, height,
            near, far,
            move_speed,
            zoom
        }
    }
}

pub type ImageFormat = PixelFormat;

#[derive(Default, Debug, PartialEq)]
pub struct Effect2D {
    program: Program,
    projection: i32,
    modelview: i32
}

impl Effect2D {
    pub fn new(position: Option<String>, pixel: Option<String>) -> Result<Effect2D, String> {
        let mut pos: Option<String> = position.clone();
        let mut pix: Option<String> = pixel.clone();
        if position == None {
            pos = Some("vec4 position(vec2 pos, mat4 mvp) {
                return mvp * vec4(pos, 0.0, 1.0);
                // return projection * modelview * vec4(pos, 0.0, 1.0);
            }".to_string());
        }
        if pixel == None {
            pix = Some("vec4 pixel(vec4 color, vec2 texcoord, sampler2D text) {
                return color * texture(text, texcoord);
            }".to_string());
        }

        let vertex = ShaderBuilder::new()
            .add_input::<Vec2>("a_Position")
            .add_input::<Vec4>("a_Color")
            .add_input::<Vec2>("a_Texcoord")
            .add_output::<Vec4>("v_Color")
            .add_output::<Vec2>("v_Texcoord")
            .add_uniform::<Mat4>("u_MVP")
            .push_pre_main(pos.unwrap().as_str())
            .set_main("void main() {
                gl_Position = position(a_Position, u_MVP);
                v_Color = a_Color;
                v_Texcoord = a_Texcoord;
            }")
            .build(ShaderType::VertexShader)
            .unwrap();

        let fragment = ShaderBuilder::new()
            .add_input::<Vec4>("v_Color")
            .add_input::<Vec2>("v_Texcoord")
            .add_uniform::<Sampler2D>("u_Texture")
            .push_pre_main(pix.unwrap().as_str())
            .set_main("void main() {
                gl_FragColor = pixel(v_Color, v_Texcoord, u_Texture);
            }")
            .build(ShaderType::FragmentShader)
            .unwrap();

        let program = Program::new().unwrap();
        program.attach_shader(&vertex);
        program.attach_shader(&fragment);
        program.link().expect("Failed to link program");

        let projection = program.get_uniform_location("u_Projection");
        let modelview = program.get_uniform_location("u_Modelview");
        // println!("{projection} {modelview}");
        Ok(Effect2D {
            program,
            projection,
            modelview
        })
    }
}

impl Effect for Effect2D {
    fn send_uniform<T: GlUniform>(&self, name: &str, value: T) {
        let location = self.program.get_uniform_location(name);
        self.program.send_uniform(location, value);
    }
}

#[derive(Default, Debug, PartialEq)]
pub struct SpriteBatch {
    vbo: VertexBuffer,
    ibo: IndexBuffer,
}

#[derive(Default, Debug)]
pub struct Render2D {
    vao: VertexArray,
    batch: SpriteBatch,
    default_effect: Effect2D,
    white_image: Image,
    default_canvas: Canvas,

    draw_mode: DrawMode,

    clear_color: Color,
    draw_color: Color,

    camera: Camera2D,
}

impl Render2D {
    pub fn clear_batch(&mut self) {
        self.batch.clear();
    }

    pub fn set_clear_color(&mut self, color: Color) {
        let v = color.to_vec4();
        self.clear_color = color;
        tea::clear_color(v.x, v.y, v.z, v.w);
    }

    pub fn set_draw_color(&mut self, color: Color) {
        let v = color.to_vec4();
        self.draw_color = color;
        tea::clear_color(v.x, v.y, v.z, v.w);
    }

    pub fn clear_color(&self) -> Color { self.clear_color }
    pub fn draw_color(&self) -> Color { self.draw_color }

    fn set_draw_mode(&mut self, mode: DrawMode) {
        if self.draw_mode != mode {
            self.draw_and_clean();
            self.draw_mode = mode;
        }
    }

    fn set_image(&mut self, image: Option<&Image>) {
        let i: &Image;
        if image == None { i = &self.white_image; }
        else { i = image.unwrap(); }
        if Texture2D::current_bind() == i.texture.get_id() { return }
        self.draw();
        self.batch.clear();
        i.texture.bind();
    }

    pub fn hello_triangle(&mut self) {
        self.set_image(None);
        self.set_draw_mode(DrawMode::Triangles);
        let mut data: Vec<Vertex2D> = vec![];
        data.push(Vertex2D::new(Vec2::new(128.0, 64.0), Vec4::new(1.0, 0.0, 0.0, 1.0), Vec2::new(0.0, 0.0)));
        data.push(Vertex2D::new(Vec2::new(64.0, 128.0), Vec4::new(0.0, 1.0, 0.0, 1.0), Vec2::new(0.0, 0.0)));
        data.push(Vertex2D::new(Vec2::new(196.0, 128.0), Vec4::new(0.0, 0.0, 1.0, 1.0), Vec2::new(0.0, 0.0)));
        self.batch.push_vec(data);
    }

    pub fn blit(&mut self, draw: &impl Drawable, src: Option<&Rect>, x: f32, y: f32) {
        self.set_draw_mode(DrawMode::Triangles);
        let texture = draw.get_handle();
        if Texture2D::current_bind() != texture.get_id() {
            self.draw_and_clean();
            texture.bind();
        }
        let mut rect = Rect::new(x, y, draw.get_width() as f32, draw.get_height() as f32);
        let mut uv: Vec4 = Vec4::new(0.0, 0.0, 1.0, 1.0);
        if src != None {
            let rr = src.unwrap();
            let width = draw.get_width() as f32;
            let height = draw.get_height() as f32;
            uv.x = rr.x / width;
            uv.y = rr.y / height;
            uv.z = (rr.x + rr.w) / width;
            uv.w = (rr.y + rr.h) / height;

            rect.w = rr.w;
            rect.h = rr.h;
        }

        if draw.flip_y() {
            uv.y = 1.0 - uv.y;
            uv.w = 1.0 - uv.w;
            // let aux = uv.y;
            // uv.y = uv.w;
            // uv.w = aux;
        }

        self.batch.add_quad(
            Vec4::new(rect.x, rect.y, uv.x, uv.y),
            Vec4::new(rect.x+rect.w, rect.y, uv.z, uv.y),
            Vec4::new(rect.x+rect.w, rect.y+rect.h, uv.z, uv.w),
            Vec4::new(rect.x, rect.y+rect.h, uv.x, uv.w),
        );
    }

    pub fn blit_scale(&mut self, draw: &impl Drawable, src: Option<&Rect>, x: f32, y: f32, sx: f32, sy: f32) {
        self.set_draw_mode(DrawMode::Triangles);
        let texture = draw.get_handle();
        if Texture2D::current_bind() != texture.get_id() {
            self.draw_and_clean();
            texture.bind();
        }
        let mut rect = Rect::new(x, y, draw.get_width() as f32, draw.get_height() as f32);
        let mut uv = Vec4::new(0.0, 0.0, 1.0, 1.0);
        if src != None {
            let rr = src.unwrap();
            let width = draw.get_width() as f32;
            let height = draw.get_height() as f32;
            uv.x = rr.x / width;
            uv.y = rr.y / height;
            uv.z = (rr.x + rr.w) / width;
            uv.w = (rr.y + rr.h) / height;

            rect.w = rr.w;
            rect.h = rr.h;
        }

        if draw.flip_y() {
            uv.y = 1.0 - uv.y;
            uv.w = 1.0 - uv.w;
        }
        let width = rect.w * sx;
        let height = rect.h * sy;
        self.batch.add_quad(
            Vec4::new(rect.x, rect.y, uv.x, uv.y),
            Vec4::new(rect.x+width, rect.y, uv.z, uv.y),
            Vec4::new(rect.x+width, rect.y+height, uv.z, uv.w),
            Vec4::new(rect.x, rect.y+height, uv.x, uv.w),
        );
    }

    pub fn blit_rotate(&mut self, draw: &impl Drawable, src: Option<&Rect>, x: f32, y: f32, angle: f32) {
        self.set_draw_mode(DrawMode::Triangles);
        let texture = draw.get_handle();
        if Texture2D::current_bind() != texture.get_id() {
            self.draw_and_clean();
            texture.bind();
        }
        let mut rect = Rect::new(x, y, draw.get_width() as f32, draw.get_height() as f32);
        let mut uv = Vec4::new(0.0, 0.0, 1.0, 1.0);
        if src != None {
            let rr = src.unwrap();
            let width = draw.get_width() as f32;
            let height = draw.get_height() as f32;
            uv.x = rr.x / width;
            uv.y = rr.y / height;
            uv.z = (rr.x + rr.w) / width;
            uv.w = (rr.y + rr.h) / height;

            rect.w = rr.w;
            rect.h = rr.h;
        }

        if draw.flip_y() {
            uv.y = 1.0 - uv.y;
            uv.w = 1.0 - uv.w;
        }
        let width = rect.w;
        let height = rect.h;
        let mut data: [Vec4; 4] = [
            Vec4::new(rect.x, rect.y, 0.0, 1.0),
            Vec4::new(rect.x+width, rect.y, 0.0, 1.0),
            Vec4::new(rect.x+width, rect.y+height, 0.0, 1.0),
            Vec4::new(rect.x, rect.y+height, 0.0, 1.0),
        ];
        /*
            Vec4::new(0.0, 0.0, 0.0, 0.0),
            Vec4::new(1.0, 0.0, 0.0, 0.0),
            Vec4::new(1.0, 1.0, 0.0, 0.0),
            Vec4::new(0.0, 1.0, 0.0, 0.0),
        */
        /*
            Vec4::new(rect.x, rect.y, 0.0, 0.0),
            Vec4::new(rect.x+width, rect.y, 0.0, 0.0),
            Vec4::new(rect.x+width, rect.y+height, 0.0, 0.0),
            Vec4::new(rect.x, rect.y+height, 0.0, 0.0),
        */
        let mut m = Mat4::identity();
        m.translate(Vec3::new(-x, -y, 0.0));
        m.rotate(angle);
        m.translate(Vec3::new(x, y, 0.0));
        // println!("{m:?}");
        for d in &mut data {
            d.apply_matrix(m);
        }
        data[0].z = uv.x;
        data[0].w = uv.y;

        data[1].z = uv.z;
        data[1].w = uv.y;

        data[2].z = uv.z;
        data[2].w = uv.w;

        data[3].z = uv.x;
        data[3].w = uv.w;

        self.batch.add_quad(
            data[0], data[1], data[2], data[3],
        );
    }

    pub fn blit_ex(&mut self, draw: &impl Drawable, src: Option<&Rect>, x: f32, y: f32, angle: f32, sx: f32, sy: f32) {
        self.set_draw_mode(DrawMode::Triangles);
        let texture = draw.get_handle();
        if Texture2D::current_bind() != texture.get_id() {
            self.draw_and_clean();
            texture.bind();
        }
        let mut rect = Rect::new(x, y, draw.get_width() as f32, draw.get_height() as f32);
        let mut uv = Vec4::new(0.0, 0.0, 1.0, 1.0);
        if src != None {
            let rr = src.unwrap();
            let width = draw.get_width() as f32;
            let height = draw.get_height() as f32;
            uv.x = rr.x / width;
            uv.y = rr.y / height;
            uv.z = (rr.x + rr.w) / width;
            uv.w = (rr.y + rr.h) / height;

            rect.w = rr.w;
            rect.h = rr.h;
        }

        if draw.flip_y() {
            uv.y = 1.0 - uv.y;
            uv.w = 1.0 - uv.w;
        }
        let width = rect.w;
        let height = rect.h;
        let mut data: [Vec4; 4] = [
            Vec4::new(rect.x, rect.y, 0.0, 1.0),
            Vec4::new(rect.x+width, rect.y, 0.0, 1.0),
            Vec4::new(rect.x+width, rect.y+height, 0.0, 1.0),
            Vec4::new(rect.x, rect.y+height, 0.0, 1.0),
        ];

        let mut m = Mat4::identity();
        m.translate(Vec3::new(-x, -y, 0.0));
        m.rotate(angle);
        m.translate(Vec3::new(x, y, 0.0));
        m.scale(Vec3::new(sx, sy, 1.0));
        // println!("{m:?}");
        for d in &mut data {
            d.apply_matrix(m);
        }
        data[0].z = uv.x;
        data[0].w = uv.y;

        data[1].z = uv.z;
        data[1].w = uv.y;

        data[2].z = uv.z;
        data[2].w = uv.w;

        data[3].z = uv.x;
        data[3].w = uv.w;

        self.batch.add_quad(
            data[0], data[1], data[2], data[3],
        );
    }

    pub fn draw_triangle(&mut self, p0: Vec2, p1: Vec2, p2: Vec2) {
        self.set_draw_mode(DrawMode::Lines);
        self.set_image(None);
        self.batch.push(Vertex2D::from_position(p0.x, p0.y));
        self.batch.push(Vertex2D::from_position(p1.x, p1.y));

        self.batch.push(Vertex2D::from_position(p1.x, p1.y));
        self.batch.push(Vertex2D::from_position(p2.x, p2.y));

        self.batch.push(Vertex2D::from_position(p2.x, p2.y));
        self.batch.push(Vertex2D::from_position(p0.x, p0.y));
    }

    pub fn fill_triangle(&mut self, p0: Vec2, p1: Vec2, p2: Vec2) {
        self.set_draw_mode(DrawMode::Triangles);
        self.set_image(None);

        let mut data: Vec<Vertex2D> = vec![];

        data.push(Vertex2D::from_position(p0.x, p0.y));
        data.push(Vertex2D::from_position(p1.x, p1.y));
        data.push(Vertex2D::from_position(p2.x, p2.y));

        self.batch.push_vec(data);
    }

    pub fn fill_rect(&mut self, rect: &Rect) {
        self.set_draw_mode(DrawMode::Triangles);
        self.set_image(None);
        self.batch.add_rect(rect);
    }

    pub fn draw_rect(&mut self, rect: &Rect) {
        self.set_draw_mode(DrawMode::Lines);
        self.set_image(None);
        let mut data: Vec<Vertex2D> = vec![];
        data.push(Vertex2D::from_position(rect.x, rect.y));
        data.push(Vertex2D::from_position(rect.x+rect.w, rect.y));

        data.push(Vertex2D::from_position(rect.x+rect.w, rect.y));
        data.push(Vertex2D::from_position(rect.x+rect.w, rect.y+rect.h));

        data.push(Vertex2D::from_position(rect.x, rect.y+rect.h));
        data.push(Vertex2D::from_position(rect.x+rect.w, rect.y+rect.h));

        data.push(Vertex2D::from_position(rect.x, rect.y));
        data.push(Vertex2D::from_position(rect.x, rect.y+rect.h));

        self.batch.push_vec(data);
    }

    pub fn draw_circle(&mut self, cx: f32, cy: f32, radius: f32) {
        self.set_draw_mode(DrawMode::Lines);
        self.set_image(None);
        let sides = 32;
        let p0 = Vec2::new(cx, cy);
        let pi2 = std::f32::consts::PI * 2.0;
        let mut data: Vec<Vertex2D> = vec![];
        for i in 0..sides {
            let tetha = (i as f32 * pi2) / sides as f32;
            let tetha2 = ((i + 1) as f32 * pi2) / sides as f32;
            let p1 = p0 + Vec2::new(tetha.cos() * radius, tetha.sin() * radius);
            let p2 = p0 + Vec2::new(tetha2.cos() * radius, tetha2.sin() * radius);
            data.push(Vertex2D::from_position(p1.x, p1.y));
            data.push(Vertex2D::from_position(p2.x, p2.y));
        }
        self.batch.push_vec(data);
    }

    pub fn fill_circle(&mut self, cx: f32, cy: f32, radius: f32) {
        self.set_draw_mode(DrawMode::Triangles);
        self.set_image(None);
        let sides = 32;
        let pi2 = std::f32::consts::PI * 2.0;
        let p0 = Vec2::new(cx, cy);
        let mut data: Vec<Vertex2D> = vec![];
        for i in 0..sides {
            let tetha = (i as f32 * pi2) / sides as f32;
            let tetha2 = ((i + 1) as f32 * pi2) / sides as f32;
                let p1 = p0 + Vec2::new(tetha.cos() * radius, tetha.sin() * radius);
            let p2 = p0 + Vec2::new(tetha2.cos() * radius, tetha2.sin() * radius);
            data.push(Vertex2D::from_position(p0.x, p0.y));
            data.push(Vertex2D::from_position(p1.x, p1.y));
            data.push(Vertex2D::from_position(p2.x, p2.y));
        }
        self.batch.push_vec(data);
    }

    fn draw(&self) {
        if self.batch.count() <= 0 { return }
        self.batch.flush();
        self.vao.bind();
        tea::draw_arrays(self.draw_mode, 0, self.batch.count() as i32);
        self.vao.unbind();
    }

    fn draw_and_clean(&mut self) {
        let mvp = self.camera().view() * self.camera.projection();
        mvp.send_uniform(0);
        self.draw();
        self.batch.clear();
    }
}

impl Render for Render2D {
    type Effect = Effect2D;
    fn new() -> Self {
        let vao = VertexArray::new().unwrap();
        let batch = SpriteBatch::new(1000).unwrap();
        // println!("{:?}", batch);

        vao.bind();
        batch.vbo.handle.bind();
        vao.setup_for::<Vertex2D>();
        vao.unbind();
        batch.vbo.handle.unbind();

        // let default_effect = Effect::from_default().unwrap();
        let default_effect = Effect2D::new(None, None).unwrap();
        let default_canvas = Canvas::default();
        let white_image = Image::new(1, 1, ImageFormat::RGBA, vec![255, 255, 255, 255]).unwrap();
        // println!("{:?}", white_image);
        let clear_color = Color::black();
        let draw_color = Color::white();
        let camera = Camera2D::new(640.0, 380.0);
        Render2D {
            vao,
            batch,
            default_effect,
            white_image,
            default_canvas,
            clear_color,
            draw_color,
            camera,
            ..Default::default()
        }
    }

    fn begin(&mut self) {
        self.set_image(None);
        self.set_canvas(None);
        self.set_effect(None);
        self.batch.clear();
        tea::enable(&[EnableFlags::Blend]);
        tea::blend_func(BlendFunc::SrcAlpha, BlendFunc::OneMinusSrcAlpha);
    }

    fn end(&self) {
        self.draw();
        tea::disable(&[EnableFlags::Blend]);
    }

    fn set_canvas(&mut self, canvas: Option<&Canvas>) {
        let c: &Canvas;
        if canvas == None { c = &self.default_canvas; }
        else { c = canvas.unwrap(); }
        if Framebuffer::current_bind() == c.framebuffer.get_id() { return }
        self.draw();
        self.batch.clear();
        c.framebuffer.bind();
        let right = c.width as f32;
        let bottom = c.height as f32;
        self.camera.set_size(right, bottom);
        tea::viewport(0, 0, c.width, c.height);
    }

    fn set_effect(&mut self, effect: Option<&Self::Effect>) {
        let e: &Self::Effect;
        if effect == None { e = &self.default_effect; }
        else { e = effect.unwrap(); }
        e.program.set_used();
        (self.camera.view() * self.camera.projection()).send_uniform(0);
    }

    fn on_resize(&mut self, width: i32, height: i32) {
        self.default_canvas.width = width;
        self.default_canvas.height = height;
        self.camera.set_size(width as f32, height as f32);
        tea::viewport(0, 0, width, height);
    }

    fn camera(&mut self) -> &mut dyn Camera {
        &mut self.camera
    }
}

/* Impl SpriteBatch */
impl SpriteBatch {
    pub fn new(count: usize) -> Result<Self, String> {
        let vbo = VertexBuffer::new(count).unwrap();
        let ibo = IndexBuffer::new(count).unwrap();
        Ok(SpriteBatch {
            vbo,
            ibo,
        })
    }

    #[cfg(target = "")]
    fn grow(&mut self) {
        let count = self.data.capacity();
        let mut v: Vec<Vertex2D> = Vec::with_capacity(count);
        self.data.append(&mut v);
        self.vbo.bind();
        self.vbo.data(self.data.len() * Vertex2D::stride(), self.data.as_ptr() as *const c_void, BufferUsage::DynamicDraw);
        self.vbo.unbind();
    }

    pub fn push(&mut self, data: Vertex2D) {
        if self.vbo.len() + 1 >= self.vbo.capacity() {
            self.vbo.grow();
            self.ibo.grow();
        }
        self.vbo.push(data);
        /*
        if self.data.len() + 1 >= self.data.capacity() {
            self.grow();
        }
        self.data.push(data);
        */
    }

    pub fn push_vec(&mut self, data: Vec<Vertex2D>) {
        if self.vbo.len() + data.len() >= self.vbo.capacity() {
            self.vbo.grow();
            self.ibo.grow();
        }
        self.vbo.push_vec(data);
    }

    pub fn add_rect(&mut self, rect: &Rect) {
        self.add_quad(
            Vec4::new(rect.x, rect.y, 0.0, 0.0),
            Vec4::new(rect.x+rect.w, rect.y, 1.0, 0.0),
            Vec4::new(rect.x+rect.w, rect.y+rect.h, 1.0, 1.0),
            Vec4::new(rect.x, rect.y+rect.h, 0.0, 1.0),
        );
    }

    pub fn add_quad(&mut self, v0: Vec4, v1: Vec4, v2: Vec4, v3: Vec4) {
        let mut data: Vec<Vertex2D> = vec![];
        data.push(Vertex2D::from_pos_uv(v0.x, v0.y, v0.z, v0.w));
        data.push(Vertex2D::from_pos_uv(v1.x, v1.y, v1.z, v1.w));
        data.push(Vertex2D::from_pos_uv(v2.x, v2.y, v2.z, v2.w));

        data.push(Vertex2D::from_pos_uv(v0.x, v0.y, v0.z, v0.w));
        data.push(Vertex2D::from_pos_uv(v2.x, v2.y, v2.z, v2.w));
        data.push(Vertex2D::from_pos_uv(v3.x, v3.y, v3.z, v3.w));
        self.push_vec(data);
    }

    pub fn flush(&self) {
        self.vbo.flush();
    }

    pub fn count(&self) -> usize { self.vbo.len() }

    pub fn clear(&mut self) {
        self.vbo.clear();
    }
}

/* Impl Vertex2D */
impl Default for Vertex2D {
    fn default() -> Self {
        Vertex2D (
            Vec2::new(0.0, 0.0),
            Vec4::new(0.0, 0.0, 0.0, 0.0),
            Vec2::new(0.0, 0.0),
        )
    }
}

impl Vertex2D {
    pub fn new(pos: Vec2, color: Vec4, texcoord: Vec2) -> Self {
        Vertex2D(
            pos,
            color,
            texcoord
        )
    }

    pub fn from_position(x: f32, y: f32) -> Self {

        Vertex2D(
            Vec2::new(x, y),
            Vec4::new(1.0, 1.0, 1.0, 1.0),
            Vec2::new(0.0, 0.0)
        )
    }
    
    pub fn from_pos_uv(x: f32, y: f32, u: f32, v: f32) -> Self {
        Vertex2D (
            Vec2::new(x, y),
            Vec4::new(1.0, 1.0, 1.0, 1.0),
            Vec2::new(u, v)
        )
    }
}
