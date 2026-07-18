#![no_std]
#![no_main]

extern crate alloc;

use alloc::string::ToString;
use alloc::vec::Vec;
use core::panic::PanicInfo;

use core::alloc::{GlobalAlloc, Layout};
use core::fmt::{self, Write};
#[global_allocator]
static ALLOCATOR: LiballocAllocator = LiballocAllocator;


unsafe extern "C" {
    fn malloc(size: usize) -> *mut u8;
    fn free(ptr: *mut u8);
    fn realloc(ptr: *mut u8, size: usize) -> *mut u8;
    fn rust_print(s: *const u8, len: usize);
    fn qemu_poweroff();

}

struct LiballocAllocator;

unsafe impl GlobalAlloc for LiballocAllocator {
    unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
        unsafe { malloc(layout.size()) }
    }
    unsafe fn dealloc(&self, ptr: *mut u8, _layout: Layout) {
        unsafe { free(ptr) }
    }
    unsafe fn realloc(&self, ptr: *mut u8, _layout: Layout, new_size: usize) -> *mut u8 {
        unsafe { realloc(ptr, new_size) }
    }
}
struct Console;

impl Write for Console {
    fn write_str(&mut self, s: &str) -> fmt::Result {
        unsafe {
            rust_print(s.as_ptr(), s.len());
        }
        Ok(())
    }
}


#[macro_export]
macro_rules! print {
    () => {{
        $crate::print(core::format_args!(""));
    }};

    ($fmt:expr) => {{
        $crate::print(core::format_args!("{}", $fmt));
    }};

    ($fmt:expr, $($arg:tt)*) => {{
        $crate::print(core::format_args!(
            concat!($fmt, ""),
            $($arg)*
        ));
    }};

}

#[macro_export]
macro_rules! println {
    () => {{
        $crate::print(core::format_args!("\n"));
    }};

    ($fmt:expr) => {{
        $crate::print(core::format_args!("{}\n", $fmt));
    }};

    ($fmt:expr, $($arg:tt)*) => {{
        $crate::print(core::format_args!(
            concat!($fmt, "\n"),
            $($arg)*
        ));
    }};
}

pub fn print(args: fmt::Arguments) {
    let mut console = Console;
    console.write_fmt(args).unwrap();
}


#[unsafe(no_mangle)]
pub extern "C" fn rust_main() -> ! {
    println!("Hello from Rust!");

    let mut v = Vec::<&str>::new();

    v.push("Hello ");
    v.push("World !");

    let max_col = 5;

    for i in 0..255 {
        if i % max_col == 0 {
            println!();
        }
        let s = ((i as u8) as char).to_string();
        print!("{} : {} \t", i, s);
    }
    println!();

    for a in v{
        print!(a);
    }
    println!();

    unsafe{ qemu_poweroff(); }

    loop {}
}

#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    print!("PANIC: ");
    if let Some(location) = info.location() {
        println!(location.file());
    }
    loop {
        unsafe { core::arch::asm!("wfi"); }
    }
}
