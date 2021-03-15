function OnCreate()
  PlayAnimation("Idle")
end

function MoveCancel()
  if Input.moveX ~= 0 or Input.moveY ~= 0 then
    PlayAnimation("Run")
  end
end

  
function Idle()
  if (Input.button1_press_age <= 10) then
    PlayAnimation("Forehand")
  elseif (Input.button2_press_age <= 10) then
    PlayAnimation("Slam")
  end
  if Input.moveX ~= 0 or Input.moveY ~= 0 then
    PlayAnimation("Run")
  end
end

function Idle_Start()
  LockRotation()
end

function Idle_End()
  UnlockRotation()
end



function Run()
  if (Input.button1_press_age <= 10) then
    PlayAnimation("Forehand")
  elseif (Input.button2_press_age <= 10) then
    PlayAnimation("Slam")
  end
  if Input.moveX == 0 and Input.moveY == 0 then
    PlayAnimation("Idle")
  end
end

function Run_Start()
  LookAtMotion()
  UnlockMotion()
  UnlockRotation()
end

function Run_22()
  PlaySound(Sounds.step)
end

function Run_47()
  PlaySound(Sounds.step)
end



function Forehand()
  if (Data.frame >= 24) then
    MoveCancel()
  end
end
function Forehand_Start()
  LockMotion()
  LookAtMouse()
  LockRotation()
end
function Forehand_End()
end
function Forehand_Last()
  UnlockMotion()
  PlayAnimation("Idle")
end
function Forehand_17()
  if (Input.button2_press_age <= 15) then
    PlayAnimation("Backhand", true)
  elseif (Input.button1_press_age <= 15) then
    PlayAnimation("Push", true)
  end
end
function Forehand_10()
  PlaySound(Sounds.whoosh1)
  AddTrauma(0.3)
end



function Backhand()
  if (Data.frame >= 40) then 
    MoveCancel()
  end
end
function Backhand_Start()
end
function Backhand_End()
  UnlockMotion()
end
function Backhand_Last()
  PlayAnimation("Idle")
end
function Backhand_18()
  PlaySound(Sounds.whoosh2)
  AddTrauma(0.8)
end



function Push()
  if (Data.frame >= 26) then 
    MoveCancel()
  end
end
function Push_Start()
end
function Push_End()
  UnlockMotion()
end
function Push_Last()
  PlayAnimation("Idle")
end
function Push_7()
  PlaySound(Sounds.whoosh1)
  AddTrauma(0.3)
end

function Slam()
  LockRotation()
  LookAtMouse()
  LockMotion()
end
function Slam_Last()
    PlayAnimation("Idle")
end

function Slam_27()
  PlaySound(Sounds.whoosh2)
end

function Slam_36()
  AddTrauma(1.0)
end

-- function Clap()
--   if Data.frame >= 20 then
--     MoveCancel()
--   end
-- end

-- function Clap_0()
--   LookAtMouse()
--   LockRotation()
--   -- LockMotion(2)
--   SetMovementSpeed(0.3)
-- end

-- function Clap_17()
--   UnlockMotion()
-- end

-- function Clap_End()
--   UnlockRotation()
--   -- UnlockMotion()
--   SetMovementSpeed(1.0)
-- end

-- function Clap_Last()
--   PlayAnimation("Idle")
-- end