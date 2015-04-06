
mod foo {

    pub fn test1() {
        println!("foo::test1()");
    }

}

mod bar {
    fn priv2() {
        println!("bar::priv2()");
    }
    macro_rules! mymacro {
        () => (
            println!("bar::mymacro()");
            //priv2();
        )
    }
    fn private() {
        println!("bar::private()");
        mymacro!();
    }
    pub fn test1() {
        println!("bar::test1()");
        super::foo::test1();    // ok
        private();
    }
}

fn main() {
    println!("Hello");
    foo::test1();
    bar::test1();
    //bar::mymacro!();
}

