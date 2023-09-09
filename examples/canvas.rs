use cafe::{CafeBuilder, Game, Color, Rect, Vec2, keyboard::Scancode};
use cafe::render::render2D::Render2D;
use cafe::render::{Render, Canvas};

extern crate cafe;

pub struct MyGame {
    canvas: Canvas,
    pos: Vec2,
    time: f32
}

impl Game for MyGame {
    type Render = Render2D;
    fn new() -> Self {
        let canvas = Canvas::new(160, 95).unwrap();
        MyGame { canvas, pos: Vec2::new(0.0, 16.0), time: 0.0 }
    }

    fn run(&mut self, dt: f32, render: &mut Self::Render) -> bool {
        let speed = 80.0 * dt;
        self.time = self.time + (dt * 30.0);
        if cafe::keyboard::is_down(Scancode::Left) {
            self.pos -= Vec2::new(speed, 0.0);
        }
        else if cafe::keyboard::is_down(Scancode::Right) {
            self.pos += Vec2::new(speed, 0.0);
        }
        render.begin();
        render.set_clear_color(Color::black());
        render.clear();

        render.set_canvas(Some(&self.canvas));
        render.clear();
        render.draw_rect(&Rect::new(self.pos.x, self.pos.y, 32.0, 64.0));
        // render.hello_triangle();
        render.set_canvas(None);

        render.blit(&self.canvas, None, 0.0, 0.0);
        render.blit_rotate(&self.canvas, None, 32.0, 64.0, self.time); 
        render.fill_circle(32.0, 64.0, 8.0);
        render.hello_triangle();

        render.end();
        true
    }
}

fn main() {
    let mut cafe = CafeBuilder::new("Canvas", 640, 380)
        .build::<MyGame>()
        .unwrap();
    cafe.run();
}
