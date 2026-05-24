with Ada.Text_IO; use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Ada.Numerics.Discrete_Random;
with Ada.Command_Line; use Ada.Command_Line;

procedure Philosophers is

    Default_Num_Philosophers : constant Integer := 5;
    Default_Meals_Per_Philosopher : constant Integer := 5;

    Num_Philosophers : Integer := Default_Num_Philosophers;
    Meals_Per_Philosopher : Integer := Default_Meals_Per_Philosopher;

    subtype Delay_Range is Integer range 1 .. 3;

    package Rand_Int is new Ada.Numerics.Discrete_Random(Delay_Range);
    Gen : Rand_Int.Generator;

    type Stats_Array is array (Natural range <>) of Integer;

    protected Stats is
        procedure Set(Id : Integer; Value : Integer);
        function Get(Id : Integer) return Integer;
    private
        Data : Stats_Array(0 .. Default_Num_Philosophers - 1) := (others => 0);
    end Stats;

    protected body Stats is
        procedure Set(Id : Integer; Value : Integer) is
        begin
            Data(Id) := Value;
        end Set;

        function Get(Id : Integer) return Integer is
        begin
            return Data(Id);
        end Get;
    end Stats;

    protected type Fork is
        entry Pick_Up;
        procedure Put_Down;
    private
        Taken : Boolean := False;
    end Fork;

    protected body Fork is
        entry Pick_Up when not Taken is
        begin
            Taken := True;
        end Pick_Up;

        procedure Put_Down is
        begin
            Taken := False;
        end Put_Down;
    end Fork;

    protected type Waiter(Max : Integer) is
        entry Request_Seat;
        procedure Leave_Seat;
    private
        Sitting : Integer := 0;
    end Waiter;

    protected body Waiter is
        entry Request_Seat when Sitting < Max is
        begin
            Sitting := Sitting + 1;
        end Request_Seat;

        procedure Leave_Seat is
        begin
            Sitting := Sitting - 1;
        end Leave_Seat;
    end Waiter;

    Forks : array (0 .. Default_Num_Philosophers - 1) of aliased Fork;
    Butler : Waiter(Default_Num_Philosophers - 1);

    task type Philosopher (Id : Integer; Left : access Fork; Right : access Fork);
    type Philosopher_Access is access Philosopher;

    task body Philosopher is
        Failed_Attempts : Integer := 0;

        procedure Think is
        begin
            Put_Line("Filozof" & Integer'Image(Id + 1) & " myśli nad sensem życia.");
            delay Duration(Rand_Int.Random(Gen));
        end Think;

        procedure Eat(Meal_No : Integer) is
        begin
            Put_Line("Filozof" & Integer'Image(Id + 1) &
                     " zjada burgera numer" & Integer'Image(Meal_No));
            delay Duration(Rand_Int.Random(Gen));
        end Eat;

    begin
        for Meal in 1 .. Meals_Per_Philosopher loop
            Think;
            Put_Line("Filozof" & Integer'Image(Id + 1) & " jest głodny i próbuje zjeść.");
            Butler.Request_Seat;

            select
                Left.Pick_Up;
            or
                delay 0.1;
                Failed_Attempts := Failed_Attempts + 1;
                Put_Line("Filozof" & Integer'Image(Id + 1) &
                         " jest za wolny i nie podniósł lewego widelca.");
                Butler.Leave_Seat;
                goto Continue_Loop;
            end select;

            select
                Right.Pick_Up;
            or
                delay 0.1;
                Failed_Attempts := Failed_Attempts + 1;
                Put_Line("Filozof" & Integer'Image(Id + 1) &
                         " jest za wolny i nie podniówł prawego widelca.");
                Left.Put_Down;
                Butler.Leave_Seat;
                goto Continue_Loop;
            end select;

            Eat(Meal);

            Left.Put_Down;
            Right.Put_Down;
            Butler.Leave_Seat;

            <<Continue_Loop>>
            null;
        end loop;

        Stats.Set(Id, Failed_Attempts);
    end Philosopher;

    Philosophers : array(0 .. Default_Num_Philosophers - 1)
      of Philosopher_Access;

begin
    Rand_Int.Reset(Gen);

    if Argument_Count >= 2 then
        Num_Philosophers := Integer'Value(Argument(1));
        Meals_Per_Philosopher := Integer'Value(Argument(2));
    end if;

    for I in 0 .. Default_Num_Philosophers - 1 loop
        Philosophers(I) :=
          new Philosopher(
            Id => I,
            Left => Forks(I)'Access,
            Right => Forks((I + 1) mod Default_Num_Philosophers)'Access
          );
    end loop;

    declare
        All_Done : Boolean;
    begin
        loop
            All_Done := True;

            for I in 0 .. Default_Num_Philosophers - 1 loop
                if not Philosophers(I).all'Terminated then
                    All_Done := False;
                end if;
            end loop;

            exit when All_Done;
            delay 0.1;
        end loop;
    end;

    Put_Line("Statystyki");
    for I in 0 .. Default_Num_Philosophers - 1 loop
        Put_Line("Filozof" & Integer'Image(I + 1) &
                 " tyle razy obszedł się smakiem =" &
                 Integer'Image(Stats.Get(I)));
    end loop;

end Philosophers;