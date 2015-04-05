
fn function() {
    println!("function()");
}

mod my {
    #[allow(dead_code)]
    pub fn public_function() {
        println!("my::public_function()");

    }

    #[allow(dead_code)]
    fn private_function() {
        println!("my::private_function()");
    }

    pub mod nested {
        #[allow(dead_code)]
        fn function() {
            println!("my::nested::function()");
        }
    }

    // cannot be called outside mod my
    mod priv_nested {
        #[allow(dead_code)]
        pub fn function() {
            println!("my::priv_nested::function()");
        }
    }
}

fn main() {
    function();
    my::public_function();
    //my::private_function();       --> error
}
