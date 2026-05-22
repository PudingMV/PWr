with Ada.Text_IO;              use Ada.Text_IO;
with Ada.Numerics.Discrete_Random;
with Ada.Command_Line;         use Ada.Command_Line;

procedure Star_Messaging is

   NUM_USERS         : Positive;
   MESSAGES_PER_USER : Positive;

   subtype User_Id is Positive;

   package Rand is
      new Ada.Numerics.Discrete_Random(User_Id);

   Gen : Rand.Generator;

   procedure Small_Delay is
   begin
      delay 0.03;
   end Small_Delay;


   type Counter_Array is array (Positive range <>) of Integer;

   protected type Counter(Size : Positive) is
      procedure Inc(Id : Positive);
      function Get(Id : Positive) return Integer;
   private
      Data : Counter_Array(1 .. Size) := (others => 0);
   end Counter;

   protected body Counter is
      procedure Inc(Id : Positive) is
      begin
         Data(Id) := Data(Id) + 1;
      end Inc;

      function Get(Id : Positive) return Integer is
      begin
         return Data(Id);
      end Get;
   end Counter;

   Counts : access Counter;

   protected type Completion is
      procedure Init(Expected : Positive);
      procedure One_Done;
      function Done return Boolean;
   private
      Count    : Natural := 0;
      Expected : Natural := 0;
   end Completion;

   protected body Completion is
      procedure Init(Expected : Positive) is
      begin
         Count := 0;
         Completion.Expected := Expected;
      end Init;

      procedure One_Done is
      begin
         Count := Count + 1;
      end One_Done;

      function Done return Boolean is
      begin
         return Count = Expected;
      end Done;
   end Completion;

   Done_Counter : Completion;

   task Server is
      entry Send(From, To_User : Positive; Msg : Integer);
      entry Stop;
   end Server;

   task body Server is
      Stopped : Boolean := False;
   begin
      loop
         select
            accept Stop do
               Stopped := True;
            end Stop;

         or
            accept Send(From, To_User : Positive; Msg : Integer) do

               Put_Line(
                  "SERVER:"
                  & Positive'Image(From)
                  & " ->"
                  & Positive'Image(To_User)
                  & " msg ="
                  & Integer'Image(Msg)
               );

               Counts.Inc(To_User);
               Small_Delay;

            end Send;

         end select;

         exit when Stopped;

      end loop;

      Put_Line("SERVER STOPPED");

   end Server;

   task type User_Task is
      entry Start(Id : Positive);
   end User_Task;

   task body User_Task is
      My_Id : Positive;
   begin

      accept Start(Id : Positive) do
         My_Id := Id;
      end Start;

      for I in 1 .. MESSAGES_PER_USER loop

         declare
            Raw     : User_Id := Rand.Random(Gen);
            Target  : Positive := Positive ((Raw mod NUM_USERS) + 1);
         begin

            Put_Line(
               "USER"
               & Positive'Image(My_Id)
               & " -> USER"
               & Positive'Image(Target)
            );

            Server.Send(My_Id, Target, I);
            Small_Delay;

         end;

      end loop;

      Put_Line("USER" & Positive'Image(My_Id) & " finished");

      Done_Counter.One_Done;

   end User_Task;


   type User_Task_Access is access User_Task;

   type User_Task_Array is array (Positive range <>) of User_Task_Access;

   type User_Task_Array_Access is access User_Task_Array;

   Users : User_Task_Array_Access;

begin

   if Argument_Count /= 2 then
      Put_Line("Usage: ./star_messaging <users> <messages>");
      return;
   end if;

   NUM_USERS := Integer'Value(Argument(1));
   MESSAGES_PER_USER := Integer'Value(Argument(2));

   Rand.Reset(Gen);

   Counts := new Counter(NUM_USERS);
   Done_Counter.Init(NUM_USERS);

   Put_Line("START");

   Users := new User_Task_Array'(1 .. NUM_USERS => null);

   for I in 1 .. NUM_USERS loop
      Users(I) := new User_Task;
   end loop;

   for I in 1 .. NUM_USERS loop
      Users(I).Start(I);
   end loop;

   loop
      exit when Done_Counter.Done;
      delay 0.01;
   end loop;

   delay 0.1;

   Server.Stop;

   Put_Line("");
   Put_Line("RESULTS");

   for I in 1 .. NUM_USERS loop
      Put_Line(
         "USER"
         & Positive'Image(I)
         & " received:"
         & Integer'Image(Counts.Get(I))
      );
   end loop;

   Put_Line("DONE");

end Star_Messaging;