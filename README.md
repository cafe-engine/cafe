# Cafe

Cafe is game engine/framework, build on top of independent modules:

- Tea for render
- Mocha for audio
- Coffee for scripting (Rust port is a WIP)
- Latte for filesystem and package (Rust port is a WIP)

The other modules i don't started even in C, so i'll start from scratch in Rust:

- Milk for UI
- Cybercafe editor

Example code:

```Rust
extern crate cafe;

use cafe::{Game, CafeBuilder, Render};
use cafe::render::render2D::Render2D;

pub struct MyGame {}

impl Game for MyGame {
    type Render = Render2D;

    fn new() -> Self {
        MyGame {}
    }

    fn setup(&mut self, _settings: &mut GameSettings) {}
    fn run(&mut self, _dt: f32, render: &mut Self::Render) -> bool {
        render.begin();
        render.clear();
        render.end();
        true
    }
}

fn main() {
    let cafe = CafeBuilder::new("Hello Game", 640, 380)
        .resizable(true)
        .build::<MyGame>()
        .unwrap();
    cafe.run();
}

```
