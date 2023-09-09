extern crate mocha;

pub use self::mocha::{AudioUsage, AudioInstance};

pub type Music = mocha::Audio;
pub type Sound = mocha::Audio;

pub fn init() {
    mocha::init();
}

pub fn deinit() {
    mocha::quit();
}

pub fn load_music(filename: &str) -> Result<Music, String> {
    let mut music = mocha::load(filename, AudioUsage::Stream).unwrap();
    music.set_loop(true);
    Ok(music)
}

pub fn load_sound(filename: &str) -> Result<Sound, String> {
    mocha::load(filename, AudioUsage::Static)
}
