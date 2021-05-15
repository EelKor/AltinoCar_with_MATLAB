function KeyReleaseCallback(~, event)

global speed;
global steer;

mode = event.Character;

switch mode
    case char(27)
       speed = 0;
        
    case(28)
        steer = steer + 10;
        if steer > 0
            steer = 0;
        end
        
        
    case(29)
        steer = steer - 10;
        if steer < 0
            steer = 0;
        end
        
end