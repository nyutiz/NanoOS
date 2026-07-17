#![no_std]
#![no_main]


mod allocator;

extern crate alloc;

use alloc::format;
use core::panic::PanicInfo;

unsafe extern "C" {
    fn rust_print(s: *const u8, len: usize);
}

pub fn println(s: &str) {
    let formatted = format!("{}\n", s);
    unsafe {
        rust_print(formatted.as_ptr(), formatted.len());
    }
}

pub fn print(s: &str) {
    unsafe {
        rust_print(s.as_ptr(), s.len());
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn rust_main() -> ! {
    println("Hello from Rust!");

    loop {

    }
}

#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    print("PANIC: ");
    if let Some(location) = info.location() {
        println(location.file());
    }
    loop {
        unsafe { core::arch::asm!("wfi"); }
    }
}
