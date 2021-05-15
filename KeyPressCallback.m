function KeyPressCallback(~, event)

global myVariable;
global myVariableX;

mode = event.Character;

switch mode
    case char(27)
       myVariable = 0;
       
    case char(30)
        myVariable = myVariable + 0.1;
        
    case char(31)
        myVariable = myVariable - 0.1;
        
    case(28)
        myVariableX = myVariableX - 0.1;
        
    case(29)
        myVariableX = myVariableX + 0.1;
        
end
