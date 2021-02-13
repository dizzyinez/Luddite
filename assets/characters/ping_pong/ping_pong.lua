function OnCreate()
  PlayAnimation("Idle")
end



function Idle()
  -- Print(Input.moveX)
  if (Input.button1) then
    PlayAnimation("Clap")
  end
  if Input.moveX ~= 0 or Input.moveY ~= 0 then
    PlayAnimation("Run")
  end
end

function Idle_Start()
  LookAtMouse()
end



function Run()
  if (Input.button1) then
    PlayAnimation("Clap")
  end
  if Input.moveX == 0 and Input.moveY == 0 then
    PlayAnimation("Idle")
  end
end

function Run_Start()
  LookAtMotion()
end

function Run_End()
end

function Run_0()
end




function Clap()
end

function Clap_0()
  LookAtMouse()
  LockRotation()
  LockMotion(2)
end

function Clap_End()
  UnlockRotation()
  UnlockMotion()
end

function Clap_Last()
  PlayAnimation("Idle")
end