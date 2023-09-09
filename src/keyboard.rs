extern crate sdl2;

pub type Scancode = sdl2::keyboard::Scancode;

const MAX_SCANCODE: usize = Scancode::Num as usize;
static mut KEY_STATE: Vec<bool> = vec![];

fn check_key(code: Scancode) -> bool {
    unsafe { KEY_STATE[code as usize] }
}

pub fn init() {
    let ptr = unsafe { sdl2::sys::SDL_GetKeyboardState(std::ptr::null_mut()) } as *mut bool;
    unsafe { KEY_STATE = Vec::from_raw_parts(ptr, MAX_SCANCODE, MAX_SCANCODE) };
}

pub fn from_name(keyname: &str) -> Option<Scancode> {
    sdl2::keyboard::Scancode::from_name(keyname)
}

pub fn get_name(code: Scancode) -> &'static str { code.name() }
pub fn is_down(code: Scancode) -> bool { check_key(code) }
pub fn is_up(code: Scancode) -> bool { !check_key(code) }
