extern crate cafe;
extern crate tea;
extern crate mocha;

use cafe::render::{*, render2D::Render2D};
use cafe::{Game, Vec4};


#[derive(Default, Debug)]
pub struct MyGame {
    running: bool,
}

impl Game for MyGame {
    type Render = Render2D;

    fn new() -> Self {
        MyGame { running: true }
    }

    fn run(&mut self, _dt: f32, render: &mut Self::Render) -> bool {
        render.setup();
        render.clear_color(Vec4::new(0.3, 0.4, 0.4, 1.0));
        render.clear();
        render.hello_triangle();
        render.draw();
        return self.running
    }
}

 fn main() {
    let mut cafe = cafe::CafeBuilder::new("Hello", 640, 380)
        .build::<MyGame>()
        .unwrap();
    cafe.run();
 }
