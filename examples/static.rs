extern crate cafe;

use cafe::{CafeBuilder, Cafe, Rect, Game};
use cafe::render::render2D::Render2D;

static mut CAFE: Option<Cafe<MyGame>> = None;

struct MyGame {
}

impl Game for MyGame {
    type Render = Render2D;
    fn new() -> Self {
        Self {}
        }

    fn run(&mut self, _dt: f32, _render: &mut Self::Render) -> bool{
        let cafe = unsafe { CAFE.as_mut().unwrap() };
        
        let mut render = cafe.get_render();
        _render.setup();

        render.draw_rect(&Rect::new(0.0, 0.0, 32.0, 32.0));

        render.fill_circle(32.0, 32.0, 16.0);

        render.draw_circle(160.0, 64.0, 64.0);

        _render.draw();
        true
    }
}

fn main() {
    let cafe = CafeBuilder::new("Static", 640, 380)
        .build::<MyGame>()
        .unwrap();
    unsafe { CAFE = Some(cafe) };
    unsafe { CAFE.as_mut().unwrap().run() };
}
