function factory(num)
    if (num > 10) then return function()
            print("greater");
        end
    else
        return function()
            print("less then/equal");
        end
    end
end

func = factory(20);
func();

func = factory(5);
func();
