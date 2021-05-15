function KeyPressCallback(~, event)

global myVariable;
global myVariableX;

mode = event.Character;

switch mode
    % ESC
    case char(27)
       myVariable = 0;
       
    % 전진 방향키   
    case char(30)
        myVariable = myVariable + 0.1;
        
    % 후진  
    case char(31)
        myVariable = myVariable - 0.1;
    
    % 좌측
    case(28)
        myVariableX = myVariableX - 0.1;
        
    % 우측    
    case(29)
        myVariableX = myVariableX + 0.1;
    
    % 스페이스바
    case(32)
        myVariable = 0;
        myVariableX = 0;
        
end
