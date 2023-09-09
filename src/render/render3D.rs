extern crate tea;

use tea::{DrawMode, GlUniform, GlUse, GlBind, GlTarget, GlObject, ClearFlags, EnableFlags, BlendFunc};
use tea::vertex::{VertexArray, VertexData, VertexFormatBuilder, VertexFormat};
use tea::impl_vertexdata;
use tea::vec::*;
use tea::glsl::{Program, ShaderBuilder, ShaderType};
use tea::texture::{Sampler2D, Texture2D};
use tea::buffer::{ArrayBuffer, ElementArrayBuffer, BufferUsage, GlBuffer};
use tea::target::Framebuffer;

use crate::render::{Render, Canvas, Camera, Buffer, Batch, Image, Effect, Movement};

impl_vertexdata!(Vertex3D, Vec3, Vec2, Vec3, Vec4);

pub type VertexBuffer = Buffer<ArrayBuffer, Vertex3D>;
pub type IndexBuffer = Buffer<ElementArrayBuffer, u32>;

impl VertexBuffer {
    pub fn new(size: usize) -> Result<Self, String> {
        let handle = ArrayBuffer::new();
        let data: Vec<Vertex3D> = Vec::with_capacity(size);
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
        let data: Vec<u32> = Vec::with_capacity(size);
        handle.bind();
        handle.data(&data, BufferUsage::StaticDraw);
        handle.unbind();
        Ok(Self {
            handle,
            data
        })
    }
}

#[derive(Default, Debug, Copy, Clone)]
pub struct Camera3D {
    position: Vec3,
    front: Vec3,
    up: Vec3,
    right: Vec3,
    world_up: Vec3,

    yaw: f32, pitch: f32,

    move_speed: f32,
    mouse_sensitivity: f32,
    zoom: f32,

    near: f32, far: f32,

    width: f32, height: f32,
}

impl Camera for Camera3D {
    fn position(&self) -> &Vec3 { &self.position }
    fn set_position(&mut self, position: Vec3) { self.position = position; }

    fn set_width(&mut self, width: f32) {
        self.width = width;
    }

    fn set_height(&mut self, height: f32) {
        self.height = height;
    }

    fn projection(&self) -> Mat4 {
        Mat4::perspective(self.zoom.to_radians(), self.width / self.height, self.near, self.far)
    }

    fn view(&self) -> Mat4 {
        Mat4::look_at(self.position, self.position + self.front, self.up)
    }

    fn move_camera(&mut self, dir: Movement, dt: f32) {
        let velocity = self.move_speed * dt;
        match dir {
            Movement::Forward => { self.position += self.front.scalar(velocity); },
            Movement::Backward => { self.position -= self.front.scalar(velocity); },
            Movement::Right => { self.position += self.right.scalar(velocity); },
            Movement::Left => { self.position -= self.right.scalar(velocity); },
            Movement::Up => { self.position += self.up.scalar(velocity); },
            Movement::Down => { self.position -= self.up.scalar(velocity); },
        }
    }
}

impl Camera3D {
    pub fn new(position: Vec3) -> Self {
        let up = Vec3::up();

        let mut camera = Self::default();
        camera.position = position;
        camera.front = Vec3::front();
        camera.up = up;
        camera.world_up = up;
        camera.yaw = -90.0;
        camera.pitch = 0.0;
        camera.move_speed = 2.5;
        camera.mouse_sensitivity = 0.1;
        camera.near = 0.1;
        camera.far = 100.0;
        camera.zoom = 45_f32;
        camera.update();

        camera
    }

    fn update(&mut self) {
        let mut front: Vec3 = Vec3::zero();
        front.x = self.yaw.to_radians().cos() * self.pitch.to_radians().cos();
        front.y = self.pitch.to_radians().sin();
        front.z = self.yaw.to_radians().sin() * self.pitch.to_radians().cos();
        self.front = front.normalize();

        self.right = self.front.cross(&self.world_up).normalize();
        self.up = self.right.cross(&self.front).normalize();
    }

    pub fn view_projection(&self) -> Mat4 {
        let projection = Mat4::perspective(self.zoom, 16.0 / 9.0, self.near, self.far);
        let view = Mat4::look_at(self.position, self.position + self.front, self.up);
        view * projection
    }
}


pub trait Light {
    fn ambient(&self) -> &Vec3;
    fn diffuse(&self) -> &Vec3;
    fn specular(&self) -> &Vec3;
}

#[repr(C)]
#[derive(Default, Debug, Copy, Clone)]
pub struct BaseLight {
    ambient: Vec3,
    diffuse: Vec3,
    specular: Vec3,
}

impl Light for BaseLight {
    fn ambient(&self) -> &Vec3 { &self.ambient }
    fn diffuse(&self) -> &Vec3 { &self.diffuse }
    fn specular(&self) -> &Vec3 { &self.specular }
}

#[repr(C)]
#[derive(Default, Debug, Copy, Clone)]
pub struct PointLight {
    position: Vec3,
    base: BaseLight,
    
    constant: f32,
    linear: f32,
    quadratic: f32,
}

impl Light for PointLight {
    fn ambient(&self) -> &Vec3 { self.base.ambient() }
    fn diffuse(&self) -> &Vec3 { self.base.diffuse() }
    fn specular(&self) -> &Vec3 { self.base.specular() }
}

#[repr(C)]
#[derive(Default, Debug, Copy, Clone)]
pub struct DirectionalLight {
    direction: Vec3,
    base: BaseLight,
}

impl Light for DirectionalLight {
    fn ambient(&self) -> &Vec3 { self.base.ambient() }
    fn diffuse(&self) -> &Vec3 { self.base.diffuse() }
    fn specular(&self) -> &Vec3 { self.base.specular() }
}

#[repr(C)]
#[derive(Default, Debug, Copy, Clone)]
pub struct Material {
    ambient: Vec3,
    diffuse: Vec3,
    specular: Vec3,
    shininess: f32,
}

#[allow(dead_code)]
#[derive(Default, Debug)]
pub struct Mesh {
    vao: VertexArray,
    vbo: VertexBuffer,
    ibo: IndexBuffer,
}

#[derive(Default, Debug, PartialEq, PartialOrd)]
pub struct Effect3D {
    program: Program,
    mvp: i32,
}

impl Effect3D {
    pub fn new(position: Option<String>, pixel: Option<String>) -> Result<Effect3D, String> {
        let mut pos = position.clone();
        let mut pix = pixel.clone();
        if position == None {
            pos = Some("vec4 position(vec3 pos, vec3 normal, mat4 mvp) {
                // return vec4(pos, 1.0);
                return mvp * vec4(pos, 1.0);
            }".to_string());
        }
        if pixel == None {
            pix = Some("vec4 pixel(vec4 color, vec2 texcoord, sampler2D text) {
                return color * texture(text, texcoord);
            }".to_string());
        }

        let vertex = ShaderBuilder::new()
            .add_input::<Vec3>("a_Position")
            .add_input::<Vec2>("a_Texcoord")
            .add_input::<Vec3>("a_Normal")
            .add_input::<Vec4>("a_Color")
            .add_output::<Vec4>("v_Color")
            .add_output::<Vec2>("v_Texcoord")
            .add_uniform::<Mat4>("u_MVP")
            .push_pre_main(pos.unwrap().as_str())
            .set_main("void main() {
                gl_Position = position(a_Position, a_Normal, u_MVP);
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
        program.link().expect("Faied to link program");

        let mvp = program.get_uniform_location("u_MVP");

        // let projection = program.get_uniform_location("u_Projection");
        // let modelview = program.get_uniform_location("u_Modelview");

        Ok(Effect3D {
            program,
            mvp,
        })
    }
}

impl Effect for Effect3D {
    fn send_uniform<T: GlUniform>(&self, name: &str, value: T) {
        let location = self.program.get_uniform_location(name);
        self.program.send_uniform(location, value);
    }
}

#[derive(Default, Debug)]
pub struct Render3D {
    default_effect: Effect3D,
    white_image: Image,
    default_canvas: Canvas,
    camera: Camera3D,

    draw_mode: DrawMode,

    clear_color: Vec4,
    draw_color: Vec4,

    plane_mesh: Mesh,
    cube_mesh: Mesh,
}

impl Render3D {
    pub fn clear_color(&mut self, color: Vec4) {
        self.clear_color = color.clone();
        tea::clear_color(color.x, color.y, color.z, color.w);
    }

    pub fn setup(&mut self) {
        self.set_effect(None);
        // self.white_texture();
        tea::enable(&[EnableFlags::Blend, EnableFlags::DepthTest]);
        tea::blend_func(BlendFunc::SrcAlpha, BlendFunc::OneMinusSrcAlpha);
    }

    fn set_draw_mode(&mut self, mode: DrawMode) {
    }

    fn set_image(&mut self, image: Option<&Image>) {
        let i: &Image;
        if image == None { i = &self.white_image; }
        else { i = image.unwrap(); }
        if Texture2D::current_bind() == i.texture.get_id() { return }
        i.texture.bind();
    }

    pub fn white_texture(&mut self) {
        self.white_image.texture.bind();
    }

    fn draw_mesh(&self, mesh: &Mesh, pos: Vec3) {
        let model = Mat4::translation_matrix(pos);
        let mvp = model * self.camera.view() * self.camera.projection();
        mvp.send_uniform(0);
        mesh.vao.bind();
        tea::draw_elements(DrawMode::Triangles, 0, mesh.ibo.len() as i32);
        mesh.vao.unbind();
    }

    pub fn draw_plane(&mut self, pos: Vec3) {
        self.draw_mesh(&self.plane_mesh, pos);
    }

    pub fn draw_cube(&mut self, pos: Vec3) {
        self.draw_mesh(&self.cube_mesh, pos);
    }

    pub fn draw(&self) {}
}

impl Render for Render3D {
    type Effect = Effect3D;
    fn new() -> Self {
        let default_effect = Effect3D::new(None, None).unwrap();
        let white_image = Image::new(1, 1, crate::render::PixelFormat::RGBA, vec![255, 255, 255, 255]).unwrap();
        let clear_color = Vec4::new(0.0, 0.0, 0.0, 0.0);
        let default_canvas = Canvas::default();
        let cube_mesh = Mesh::cube();

        let camera = Camera3D::new(Vec3::new(0.0, 0.0, 3.0));

        tea::enable(&[EnableFlags::Blend, EnableFlags::DepthTest]);
        Render3D {
            default_effect,
            white_image,
            default_canvas,
            clear_color,
            cube_mesh,
            camera,
            ..Default::default()
        }
    }

    fn clear(&self) { tea::clear(&[ClearFlags::ColorBufferBit, ClearFlags::DepthBufferBit]); }

    fn camera(&mut self) -> &mut dyn Camera {
        &mut self.camera
    }

    fn begin(&mut self) {
        self.set_canvas(None);
        self.set_effect(None);
        self.set_image(None);
    }

    fn end(&self) {
        self.draw();
    }
    
    fn set_canvas(&mut self, canvas: Option<&Canvas>) {
        let c: &Canvas;
        if canvas == None { c = &self.default_canvas; }
        else { c = canvas.unwrap(); }
        if Framebuffer::current_bind() == c.framebuffer.get_id() { return }
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
}

// Mesh impl


impl Mesh {
    fn cube() -> Mesh {
        let vao = VertexArray::new().unwrap();
        let mut vbo = VertexBuffer::new(24).unwrap();
        let mut ibo = IndexBuffer::new(36).unwrap();
        let cube_positions: Vec<f32> = vec![
            // Front face
            -0.5, -0.5,  0.5,
             0.5, -0.5,  0.5,
             0.5,  0.5,  0.5,
            -0.5,  0.5,  0.5,
            
            // Back face
            -0.5, -0.5, -0.5,
             0.5, -0.5, -0.5,
             0.5,  0.5, -0.5,
            -0.5,  0.5, -0.5,
            
            // Left face
            -0.5, -0.5, -0.5,
            -0.5, -0.5,  0.5,
            -0.5,  0.5,  0.5,
            -0.5,  0.5, -0.5,
            
            // Right face
             0.5, -0.5, -0.5,
             0.5, -0.5,  0.5,
             0.5,  0.5,  0.5,
             0.5,  0.5, -0.5,
            
            // Top face
            -0.5,  0.5,  0.5,
             0.5,  0.5,  0.5,
             0.5,  0.5, -0.5,
            -0.5,  0.5, -0.5,
            
            // Bottom face
            -0.5, -0.5,  0.5,
             0.5, -0.5,  0.5,
             0.5, -0.5, -0.5,
            -0.5, -0.5, -0.5,
        ];

        let indices: Vec<u32> = vec![
            // Front face
            0, 1, 2, 0, 2, 3,
            
            // Back face
            4, 5, 6, 4, 6, 7,
            
            // Left face
            8, 9, 10, 8, 10, 11,
            
            // Right face
            12, 13, 14, 12, 14, 15,
            
            // Top face
            16, 17, 18, 16, 18, 19,
            
            // Bottom face
            20, 21, 22, 20, 22, 23,
        ];

        ibo.push_vec(indices);
        ibo.flush();

        let mut vertices: Vec<Vertex3D> = vec![];

        for i in 0..24 {
            let vindex = i*3;
            let x = cube_positions[vindex];
            let y = cube_positions[vindex+1];
            let z = cube_positions[vindex+2];

            vertices.push(Vertex3D::from_position(x, y, z));
        }
        vbo.push_vec(vertices);
        vbo.flush();

        vao.bind();
        vbo.handle.bind();
        ibo.handle.bind();
        vao.setup_for::<Vertex3D>();
        vao.unbind();
        vbo.handle.unbind();
        ibo.handle.unbind();
        Mesh {
            vao,
            vbo,
            ibo,
        }
    }
}

impl Default for Vertex3D {
    fn default() -> Self {
        Vertex3D (
            Vec3::new(0.0, 0.0, 0.0),
            Vec2::new(0.0, 0.0),
            Vec3::new(0.0, 0.0, 0.0),
            Vec4::new(1.0, 1.0, 1.0, 1.0),
        )
    }
}

impl Vertex3D {
    pub fn from_position(x: f32, y: f32, z: f32) -> Self {
        Vertex3D(
            Vec3::new(x, y, z),
            Vec2::new(0.0, 0.0),
            Vec3::new(0.0, 0.0, 0.0),
            Vec4::new(1.0, 1.0, 1.0, 1.0),
        )
    }
}
