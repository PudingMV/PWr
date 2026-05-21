with Ada.Command_Line;
with Ada.Integer_Text_IO;
with Ada.Real_Time;
with Ada.Strings.Fixed;
with Ada.Text_IO;

procedure Dining_Philosophers is

   use Ada.Command_Line;
   use Ada.Integer_Text_IO;
   use Ada.Real_Time;
   use Ada.Strings.Fixed;
   use Ada.Text_IO;

   Think_Min : constant Duration := 0.01;
   Think_Max : constant Duration := 0.05;
   Eat_Time  : constant Duration := 0.02;

   --  Próg: dłuższe oczekiwanie na Request_Meal = jedno „nie od razu”.
   Long_Wait : constant Time_Span := Milliseconds (5);

   function Img (N : Integer) return String is
     (Trim (Integer'Image (N), Ada.Strings.Left));

   procedure Usage is
   begin
      Put_Line ("Usage: dining_philosophers <num_philosophers> <meals_each>");
   end Usage;

begin

   if Argument_Count /= 2 then
      Usage;
      return;
   end if;

   declare

      Num_Philosophers : constant Positive := Positive'Value (Argument (1));
      Meals_Target     : constant Positive := Positive'Value (Argument (2));

      subtype Phil_Id is Positive range 1 .. Num_Philosophers;

      protected Logger is
         procedure Log (Message : String);
      private
         Line_No : Natural := 0;
      end Logger;

      protected body Logger is
         procedure Log (Message : String) is
         begin
            Line_No := Line_No + 1;
            Put_Line (Trim (Natural'Image (Line_No), Ada.Strings.Left) & " | " & Message);
         end Log;
      end Logger;

      protected Stats is
         procedure Note_Long_Wait (Id : Phil_Id);
         procedure Dump;
      private
         Long_Waits : array (Phil_Id) of Natural := (others => 0);
      end Stats;

      protected body Stats is
         procedure Note_Long_Wait (Id : Phil_Id) is
         begin
            Long_Waits (Id) := Long_Waits (Id) + 1;
         end Note_Long_Wait;

         procedure Dump is
            Total_Wait_Events : Natural := 0;
            Minv                : Natural := Natural'Last;
            Maxv                : Natural := 0;
            Sum                 : Natural := 0;
         begin
            New_Line;
            Put_Line ("=== Podsumowanie (dlugie oczekiwanie na stol/widelce) ===");
            for I in Phil_Id loop
               Put_Line
                 ("Filozof " & Img (Integer (I)) & ": "
                  & Trim (Natural'Image (Long_Waits (I)), Ada.Strings.Left)
                  & " x oczekiwanie > "
                  & Trim (Duration'Image (To_Duration (Long_Wait)), Ada.Strings.Left)
                  & " s");
               Total_Wait_Events := Total_Wait_Events + Long_Waits (I);
               Sum               := Sum + Long_Waits (I);
               if Long_Waits (I) < Minv then
                  Minv := Long_Waits (I);
               end if;
               if Long_Waits (I) > Maxv then
                  Maxv := Long_Waits (I);
               end if;
            end loop;
            New_Line;
            Put_Line ("Laczna liczba ""dlugich"" oczekiwan: " & Img (Integer (Total_Wait_Events)));
            if Num_Philosophers > 0 then
               Put_Line
                 ("Srednia na filozofa: "
                  & Img (Integer (Sum / Num_Philosophers)));
            end if;
            New_Line;
            if Maxv = 0 then
               Put_Line
                 ("Sprawiedliwosc: wszyscy dostawali dostep rownie szybko (wg progu).");
            elsif Maxv - Minv <= 1 then
               Put_Line
                 ("Sprawiedliwosc: rozklad opoznien wyglada w miare rownomiernie (max-min <= 1).");
            else
               Put_Line
                 ("Sprawiedliwosc: widoczne roznice miedzy filozofami (max-min = "
                  & Img (Integer (Maxv - Minv))
                  & ").");
            end if;
         end Dump;
      end Stats;

      protected Table is
         procedure Want_To_Eat (Id : Phil_Id);
         entry Request_Meal (Id : Phil_Id);
         procedure Release_Forks (Id : Phil_Id);
      private
         Queue     : array (Phil_Id) of Phil_Id;
         Queue_Len : Natural range 0 .. Num_Philosophers := 0;
         In_Queue  : array (Phil_Id) of Boolean := (others => False);
         Fork_Free : array (Phil_Id) of Boolean := (others => True);
         function Head return Phil_Id is (Queue (1));
         function Left (Id : Phil_Id) return Phil_Id is (Id);
         function Right (Id : Phil_Id) return Phil_Id is
           (if Id = Num_Philosophers then 1 else Id + 1);
         function Both_Forks_Free (Id : Phil_Id) return Boolean is
           (Fork_Free (Left (Id)) and then Fork_Free (Right (Id)));
      end Table;

      protected body Table is

         procedure Want_To_Eat (Id : Phil_Id) is
         begin
            if not In_Queue (Id) then
               Queue_Len := Queue_Len + 1;
               Queue (Queue_Len) := Id;
               In_Queue (Id) := True;
            end if;
         end Want_To_Eat;

         entry Request_Meal (Id : Phil_Id) when Queue_Len > 0
           and then Head = Id
           and then Both_Forks_Free (Id) is
         begin
            In_Queue (Id) := False;
            if Queue_Len > 1 then
               for J in 1 .. Queue_Len - 1 loop
                  Queue (J) := Queue (J + 1);
               end loop;
            end if;
            Queue_Len := Queue_Len - 1;

            Fork_Free (Left (Id))  := False;
            Fork_Free (Right (Id)) := False;
         end Request_Meal;

         procedure Release_Forks (Id : Phil_Id) is
         begin
            Fork_Free (Left (Id))  := True;
            Fork_Free (Right (Id)) := True;
         end Release_Forks;

      end Table;

      task type Philosopher (Id : Phil_Id);

      task body Philosopher is
         Meals_Eaten : Natural := 0;
         T0            : Time;
      begin
         loop
            exit when Meals_Eaten >= Meals_Target;

            Logger.Log ("F" & Img (Integer (Id)) & " mysli");
            delay Think_Min + Duration (Float (Meals_Eaten mod 7) * 0.001);

            Table.Want_To_Eat (Id);
            T0 := Clock;
            Table.Request_Meal (Id);
            if Clock - T0 > Long_Wait then
               Stats.Note_Long_Wait (Id);
            end if;

            Meals_Eaten := Meals_Eaten + 1;
            Logger.Log
              ("F" & Img (Integer (Id)) & " je (posilek "
               & Img (Meals_Eaten) & "/" & Img (Integer (Meals_Target)) & ")");

            delay Eat_Time;

            Table.Release_Forks (Id);
            Logger.Log ("F" & Img (Integer (Id)) & " konczy posilek");
         end loop;

         Logger.Log ("F" & Img (Integer (Id)) & " konczy prace");
      end Philosopher;

      type Phil_Array is array (Phil_Id) of Philosopher;

   begin
      declare
         Crew : Phil_Array := (others => <>);
         pragma Unreferenced (Crew);
      begin
         null;
      end;
      Stats.Dump;
   end;

exception
   when others =>
      Usage;
      raise;
end Dining_Philosophers;