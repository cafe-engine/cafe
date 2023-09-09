extern crate sdl2;
extern crate tea;
extern crate latte;

pub mod audio;
pub mod render;
pub mod keyboard;
pub mod mouse;
pub mod joystick;

use keyboard::Scancode;
pub use tea::vec::*;
use render::Render;
use sdl2::{video::{Window, WindowBuilder, GLContext, GLProfile}, EventPump, event::{Event, WindowEvent}, TimerSubsystem, keyboard::{Keycode, Mod}};

pub use sdl2::joystick::HatState;
pub use sdl2::controller::{Axis, Button};

pub type Point2D = Vec2;
pub type Point3D = Vec3;

#[derive(Default, Debug, Copy, Clone, PartialOrd, PartialEq)]
pub struct Rect {
    pub x: f32, pub y: f32,
    pub w: f32, pub h: f32
}

impl Rect {
    pub fn new(x: f32, y: f32, w: f32, h: f32) -> Rect {
        Rect { x, y, w, h }
    }
}

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

pub struct Cafe<T: Game> {
    running: bool,
    window: Window,
    timer: TimerSubsystem,
    _gl_context: GLContext,
    render: Option<T::Render>,
    event_pump: EventPump,

    current_time: u32,
        // game: dyn Game
}

#[derive(Default, Debug)]
pub struct GameSettings {
    pub window: WindowSettings
}

#[derive(Debug)]
pub struct WindowSettings {
    pub title: String,
    pub width: i32, pub height: i32,
    pub resizable: bool,
    pub borderless: bool
}

impl Default for WindowSettings {
    fn default() -> Self {
        WindowSettings {
            title: "cafe 0.1.0".to_string(),
            width: 640, height: 380,
            resizable: false,
            borderless: false,
        }
    }
}

pub struct CafeBuilder {
    sdl: sdl2::Sdl,
    video: sdl2::VideoSubsystem,
    window: WindowBuilder
}

impl CafeBuilder {
    pub fn new(title: &str, width: u32, height: u32) -> Self {
        let sdl = sdl2::init().unwrap();
        let video = sdl.video().unwrap();

        let mut window = video.window(title, width, height);
        window.position_centered().opengl();

        let gl_attr = video.gl_attr();
        gl_attr.set_context_profile(GLProfile::Core);
        gl_attr.set_context_version(3, 3);

        CafeBuilder {
            sdl,
            video,
            window,
        }
    }

    #[doc = "Resizable window"]
    pub fn resizable(&mut self) -> &mut Self {
        self.window.resizable();
        self
    }

    #[doc = "Borderless window"]
    pub fn borderless(&mut self) -> &mut Self {
        self.window.borderless();
        self
    }

    #[doc = "Start window as hidden"]
    pub fn hidden(&mut self) -> &mut Self {
        self.window.hidden();
        self
    }

    #[doc = "Real fullscreen mode"]
    pub fn fullscreen(&mut self) -> &mut Self {
        self.window.fullscreen();
        self
    }
    
    #[doc = "Fake fullscreen mode (borderless maximized window)"]
    pub fn fullscreen_desktop(&mut self) -> &mut Self {
        self.window.fullscreen_desktop();
        self
    }

    #[doc = "Build Cafe context"]
    pub fn build<T: Game>(&mut self) -> Result<Cafe<T>, String> {

        let sdl_context = &self.sdl;
        let video_subsystem = &self.video;

        let window = self.window.build().unwrap();

        let sz = window.size();
        let size = (sz.0 as i32, sz.1 as i32);
        
        let _gl_context = window.gl_create_context().expect("Failed to create GLContext");
        tea::load_with(|name| video_subsystem.gl_get_proc_address(name) as *const _);

        let event_pump = sdl_context.event_pump().unwrap();

        let mut render = Some(T::create_render());
        render.as_mut().expect("Failed to create render").on_resize(size.0, size.1);

        tea::viewport(0, 0, size.0, size.1);

        crate::keyboard::init();
        crate::audio::init();

        let timer = sdl_context.timer().unwrap();
        
        // mocha::init();
        Ok(Cafe {
            running: true,
            window,
            render,
            _gl_context,
            event_pump,
            timer,
            current_time: 0
        })
    }
}

pub trait Game {
    type Render: Render;
    fn new() -> Self;
    fn create_render() -> Self::Render { Self::Render::new() }
    fn run(&mut self, _dt: f32, _render: &mut Self::Render) -> bool { false }
    // fn update(&mut self, _dt: f32) {}
    // fn draw(&mut self, _render: &mut Self::Render) {}

    fn quit(&self) -> bool { true }
    fn window_close(&self) {}
    fn window_resized(&mut self, _width: i32, _height: i32) {}
    fn window_moved(&mut self, _x: i32, _y: i32) {}
    fn window_minimized(&mut self) {}
    fn window_maximized(&mut self) {}

    fn key_down(&mut self, _keycode: Option<Keycode>, _scancode: Option<Scancode>, _repeat: bool, _keymod: Mod) {}
    fn key_up(&mut self, _keycode: Option<Keycode>, _scancode: Option<Scancode>, _repeat: bool, _keymod: Mod) {}
    fn joystick_added(&mut self, _which: u32) {}
    fn joystick_removed(&mut self, _which: u32) {}
    fn controller_added(&mut self, _which: u32) {}
    fn controller_removed(&mut self, _which: u32) {}

    fn mouse_motion(&mut self, _x: i32, _y: i32, _xrel: i32, _yrel: i32) {}
    fn mouse_wheel(&mut self, _scroll_x: f32, _scroll_y: f32, _flipped: bool) {}

    fn text_editing(&mut self, _text: String, _start: i32, _length: i32) {}
    fn text_input(&mut self, _text: String) {}

    fn joy_button_up(&mut self, _which: u32, _button: u8) {}
    fn joy_button_down(&mut self, _which: u32, _button: u8) {}
    fn joy_hat_motion(&mut self, _which: u32, _hat: u8, _state: HatState) {}
    fn joy_ball_motion(&mut self, _which: u32, _ball: u8, _xrel: i16, _yrel: i16) {}
    fn joy_axis_motion(&mut self, _which: u32, _axis: u8, _value: i16) {}

    fn controller_button_up(&mut self, _which: u32, _button: Button) {}
    fn controller_button_down(&mut self, _which: u32, _button: Button) {}
    fn controller_axis_motion(&mut self, _which: u32, _axis: Axis, _value: i16) {}
    fn controller_remapped(&mut self, _which: u32) {}

    fn drop_file(&mut self, _filename: String) {}
    fn drop_text(&mut self, _text: String) {}
    fn drop_begin(&mut self) {}
    fn drop_complete(&mut self) {}

    fn finger_up(&mut self, _touch_id: i64, _finger_id: i64, _x: f32, _y: f32, _dx: f32, _dy: f32, _pressure: f32) {}
    fn finger_down(&mut self, _touch_id: i64, _finger_id: i64, _x: f32, _y: f32, _dx: f32, _dy: f32, _pressure: f32) {}
    fn finger_motion(&mut self, _touch_id: i64, _finger_id: i64, _x: f32, _y: f32, _dx: f32, _dy: f32, _pressure: f32) {}
    fn dollar_record(&mut self, _touch_id: i64, _gesture_id: i64, _num_fingers: u32, _error: f32, _x: f32, _y: f32) {}
    fn dollar_gesture(&mut self, _touch_id: i64, _gesture_id: i64, _num_fingers: u32, _error: f32, _x: f32, _y: f32) {}
    fn clipboard_update(&mut self) {}
}

impl<T: Game> Cafe<T> {
    pub fn run(&mut self) {
        let mut game = T::new();
        let render = self.render.as_mut().unwrap();
        let mut running = true;
        while running {
            let last_time = self.current_time;
            self.current_time = self.timer.ticks();

            let delta = (self.current_time - last_time) as f32 / 1000.0;

            running = game.run(delta, render);

            for event in self.event_pump.poll_iter() {
                match event {
                    Event::Quit {..} => { running = !game.quit(); },
                    Event::Window { win_event, .. } => {
                        match win_event {
                            WindowEvent::Close => {
                                game.window_close();
                            },
                            WindowEvent::Moved(x, y) => {
                                game.window_moved(x, y);
                            },
                            WindowEvent::Resized(w, h) => {
                                render.on_resize(w, h);
                                game.window_resized(w, h);
                            },
                            WindowEvent::Minimized => {
                                game.window_minimized();
                            },
                            WindowEvent::Maximized => {
                                game.window_maximized();
                            },
                            _ => {}
                        }
                    },
                    Event::KeyDown { keycode, scancode, repeat, keymod, .. } => { game.key_down(keycode, scancode, repeat, keymod); },
                    Event::KeyUp { keycode, scancode, repeat, keymod, .. } => { game.key_up(keycode, scancode, repeat, keymod); },
                    Event::JoyDeviceAdded { which, .. } => { game.joystick_added(which); },
                    Event::JoyDeviceRemoved { which, .. } => { game.joystick_removed(which); },
                    Event::JoyButtonUp { which, button_idx, .. } => { game.joy_button_up(which, button_idx); },
                    Event::JoyButtonDown { which, button_idx, .. } => { game.joy_button_down(which, button_idx); },
                    Event::JoyHatMotion { which, hat_idx, state, .. } => { game.joy_hat_motion(which, hat_idx, state); },
                    Event::JoyBallMotion { which, ball_idx, xrel, yrel, .. } => { game.joy_ball_motion(which, ball_idx, xrel, yrel); },
                    Event::JoyAxisMotion { which, axis_idx, value, .. } => { game.joy_axis_motion(which, axis_idx, value); },
                    Event::ControllerDeviceAdded { which, .. } => { game.controller_added(which); },
                    Event::ControllerDeviceRemoved { which, .. } => { game.controller_removed(which); },
                    Event::ControllerButtonUp { which, button, .. } => { game.controller_button_up(which, button); },
                    Event::ControllerButtonDown { which, button, .. } => { game.controller_button_down(which, button); },
                    Event::ControllerAxisMotion { which, axis, value, .. } => { game.controller_axis_motion(which, axis, value); },
                    Event::ControllerDeviceRemapped { which, .. } => { game.controller_remapped(which); },
                    Event::MouseMotion { x, y, xrel, yrel, .. } => { game.mouse_motion(x, y, xrel, yrel); },
                    Event::MouseWheel { direction, x, y, .. } => { game.mouse_wheel(x as f32, y as f32, direction == sdl2::mouse::MouseWheelDirection::Flipped); },
                    Event::TextEditing { text, start, length, .. } => { game.text_editing(text, start, length); },
                    Event::TextInput { text, .. } => { game.text_input(text); },
                    Event::DropFile { filename, .. } => { game.drop_file(filename); },
                    Event::DropText { filename, .. } => { game.drop_text(filename); },
                    Event::DropBegin { .. } => { game.drop_begin(); },
                    Event::DropComplete { .. } => { game.drop_complete(); },
                    Event::FingerUp { touch_id, finger_id, x, y, dx, dy, pressure, .. } => { game.finger_up(touch_id, finger_id, x, y, dx, dy, pressure); },
                    Event::FingerDown { touch_id, finger_id, x, y, dx, dy, pressure, .. } => { game.finger_down(touch_id, finger_id, x, y, dx, dy, pressure); },
                    Event::FingerMotion { touch_id, finger_id, x, y, dx, dy, pressure, .. } => { game.finger_motion(touch_id, finger_id, x, y, dx, dy, pressure); },
                    Event::DollarRecord { touch_id, gesture_id, num_fingers, error, x, y, .. } => { game.dollar_record(touch_id, gesture_id, num_fingers, error, x, y); },
                    Event::DollarGesture { touch_id, gesture_id, num_fingers, error, x, y, .. } => { game.dollar_gesture(touch_id, gesture_id, num_fingers, error, x, y); },
                    Event::ClipboardUpdate { .. } => { game.clipboard_update(); },
                    _ => {}
                }
            }

            // self.game.update(delta);
            // self.game.draw(render);
            self.window.gl_swap_window();
        }
    }
    
    pub fn get_window(&self) -> &Window { &self.window }
    pub fn get_render(&mut self) -> &mut T::Render { self.render.as_mut().unwrap() }

    pub fn is_running(&self) -> bool { self.running }
}

impl<T: Game> Drop for Cafe<T> {
    fn drop(&mut self) {
        crate::audio::deinit();
    }
}
