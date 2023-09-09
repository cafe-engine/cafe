use cafe::{audio::Music, Game, render::{Render, render2D::Render2D}, CafeBuilder};
use mocha::AudioInstance;

extern crate cafe;
extern crate mocha;

pub struct MyGame {
    music: Music
}

impl Game for MyGame {
    type Render = Render2D;
    fn new() -> Self {
        let music = cafe::audio::load_sound("assets/som.wav").unwrap();
        let instance = music.play();
        MyGame { music }
    }

    fn run(&mut self, dt: f32, render: &mut Self::Render) -> bool {
        render.setup();
        render.clear();
        render.draw();
        true
    }
}

fn main() {
    let mut cafe = CafeBuilder::new("Audio", 640, 380)
        .build::<MyGame>()
        .unwrap();
    cafe.run();
}
