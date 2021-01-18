function OnCreate()
  PlayAnimation("Idle")
end

function Idle()
  -- Print(Input.moveX)
  if Input.moveX ~= 0 or Input.moveY ~= 0 then
    PlayAnimation("Run")
  end
end

function Run()
  if Input.moveX == 0 and Input.moveY == 0 then
    PlayAnimation("Idle")
  end
end
