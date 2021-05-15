function KeyReleaseCallback(~, event)

global myVariable;
global myVariableX;

mode = event.Character;

switch mode
    case char(27)
       myVariable = 0;
        
    case(28)
        myVariableX = 0;
        
    case(29)
        myVariableX = 0;
        
end