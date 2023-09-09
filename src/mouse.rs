extern crate sdl2;

pub type MouseButton = sdl2::mouse::MouseButton;

pub fn position() -> (i32, i32) {
    let mut x: i32 = 0;
    let mut y: i32 = 0;
    unsafe {
        sdl2::sys::SDL_GetMouseState(&mut x, &mut y);
    }
    (x, y)
}

pub fn set_position(x: i32, y: i32) {
    unsafe {
        sdl2::sys::SDL_WarpMouseGlobal(x, y);
    }
}

pub fn relative_position() -> (i32, i32) {
    let mut x: i32 = 0;
    let mut y: i32 = 0;
    unsafe {
        sdl2::sys::SDL_GetRelativeMouseState(&mut x, &mut y);
    }
    (x, y)
}

pub fn is_down(button: MouseButton) -> bool {
    let btn = button as u32;
    let state = unsafe { sdl2::sys::SDL_GetMouseState(std::ptr::null_mut(), std::ptr::null_mut()) };
    (state & btn) != 0
}

pub fn is_up(button: MouseButton) -> bool {
    let btn = button as u32;
    let state = unsafe { sdl2::sys::SDL_GetMouseState(std::ptr::null_mut(), std::ptr::null_mut()) };
    (state & btn) == 0
}
