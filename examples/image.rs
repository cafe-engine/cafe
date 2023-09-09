extern crate cafe;

use cafe::render::Render;
use cafe::{Game, CafeBuilder};
use cafe::render::Image;
use cafe::render::render2D::Render2D;


pub struct MyGame {
    image: Image
}

impl Game for MyGame {
    type Render = Render2D;
    
    fn new() -> Self {
        let image = Image::load("assets/penguin.jpg").unwrap();
        image.pixel_art();
        MyGame { image }
    }

    fn run(&mut self, _dt: f32, render: &mut Self::Render) -> bool {
        render.setup();
        render.clear_color(cafe::Vec4::new(0.2, 0.3, 0.3, 1.0));
        render.clear();

        render.blit(&self.image, None, 0.0, 0.0);

        render.draw();
        true
    }
}

fn main() {
    let mut cafe = CafeBuilder::new("Image", 640, 380)
        .build::<MyGame>()
        .unwrap();
    cafe.run();
}
