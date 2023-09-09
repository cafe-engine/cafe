use cafe::{CafeBuilder, Game, Vec4, Vec3};
use cafe::render::render3D::Render3D;
use cafe::render::Render;

extern crate cafe;

pub struct MyGame {
    positions: Vec<Vec3>
}

impl Game for MyGame {
    type Render = Render3D;
    fn new() -> Self {
        let positions: Vec<Vec3> = vec![
            Vec3::new(1.0, 0.0, -3.0),
            Vec3::new(-2.5, -2.0, -5.0),
            Vec3::new(-2.5, -2.0, -10.0),
            Vec3::new(0.0, -3.0, -7.0),
        ];
        MyGame { positions }
    }

    fn run(&mut self, dt: f32, render: &mut Self::Render) -> bool {
        render.begin();
        render.clear_color(Vec4::new(0.3, 0.4, 0.4, 1.0));
        render.clear();

        for pos in &self.positions {
            render.draw_cube(*pos);
        }

        render.end();
        true
    }
}

fn main() {
    let mut cafe = CafeBuilder::new("Cube", 640, 380)
        .build::<MyGame>()
        .unwrap();
    cafe.run();
}

