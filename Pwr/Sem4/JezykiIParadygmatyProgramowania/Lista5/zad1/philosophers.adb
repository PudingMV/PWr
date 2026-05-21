with Ada.Text_IO; use Ada.Text_IO;
with Ada.Numerics.Discrete_Random;

procedure Philosophers is

   N_Philosophers : constant Integer := 5;
   Meals          : constant Integer := 5;

   subtype Philosopher_Id is Integer range 1 .. N_Philosophers;

   -----------------------------
   -- Widelce
   -----------------------------
   type Fork_Array is array (Philosopher_Id) of Boolean;

   -----------------------------
   -- MONITOR
   -----------------------------
   protected type Table is
      entry Take_Forks (Id : Philosopher_Id);
      procedure Put_Forks (Id : Philosopher_Id);
   private
      Fork_Free : Fork_Array := (others => True);

      function Left  (Id : Philosopher_Id) return Philosopher_Id;
      function Right (Id : Philosopher_Id) return Philosopher_Id;
   end Table;

   protected body Table is

      function Left (Id : Philosopher_Id) return Philosopher_Id is
      begin
         return Id;
      end;

      function Right (Id : Philosopher_Id) return Philosopher_Id is
      begin
         if Id = N_Philosophers then
            return 1;
         else
            return Id + 1;
         end if;
      end;

      entry Take_Forks (Id : Philosopher_Id)
        when Fork_Free(Left(Id)) and Fork_Free(Right(Id))
      is
      begin
         Fork_Free(Left(Id))  := False;
         Fork_Free(Right(Id)) := False;

         Put_Line("Filozof" & Integer'Image(Id) & " zabral widelce");
      end Take_Forks;

      procedure Put_Forks (Id : Philosopher_Id) is
      begin
         Fork_Free(Left(Id))  := True;
         Fork_Free(Right(Id)) := True;

         Put_Line("Filozof" & Integer'Image(Id) & " odlozyl widelce");
      end Put_Forks;

   end Table;

   Shared_Table : Table;

   -----------------------------
   -- RNG
   -----------------------------
   package Rand is new Ada.Numerics.Discrete_Random(Integer);

   -----------------------------
   -- TASK filozofa (BEZ ID DISCRIMINANT!)
   -----------------------------
   task type Philosopher;

   task body Philosopher is
      Id : Philosopher_Id;  -- <- ustawiane przez Init
      G  : Rand.Generator;
      Failed : Integer := 0;
   begin

      -- czekamy aż main przypisze ID
      accept Start (My_Id : Philosopher_Id) do
         Id := My_Id;
      end Start;

      Rand.Reset(G, Id);

      for Meal in 1 .. Meals loop

         Put_Line("Filozof" & Integer'Image(Id) & " mysli");

         delay Duration(Rand.Random(G) mod 3 + 1);

         Put_Line("Filozof" & Integer'Image(Id) & " chce jesc");

         select
            Shared_Table.Take_Forks(Id);
         or
            delay 0.2;
            Failed := Failed + 1;
            Shared_Table.Take_Forks(Id);
         end select;

         Put_Line("Filozof" & Integer'Image(Id) & " je");

         delay Duration(Rand.Random(G) mod 2 + 1);

         Shared_Table.Put_Forks(Id);

      end loop;

      Put_Line("--------------------------------");
      Put_Line("Filozof" & Integer'Image(Id) & " zakonczyl");
      Put_Line("Nieudane proby: " & Integer'Image(Failed));
      Put_Line("--------------------------------");

   end Philosopher;

   -----------------------------
   -- TASKI
   -----------------------------
   type Philosopher_Array is array (Philosopher_Id) of Philosopher;

   Philosophers : Philosopher_Array;

begin

   Put_Line("Start symulacji filozofow");

   -- inicjalizacja ID (POPRAWNE W ADA)
   for I in Philosopher_Id loop
      Philosophers(I).Start(I);
   end loop;

end Philosophers;