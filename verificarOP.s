.text
.arm
.global verificarOP

CMP     R0, #1        
    BLT     fuera_rango  
    CMP     R0, #3          
    BGT     fuera_rango    
    B       dentro_rango  

fuera_rango:
    MOV R0, #1
    B       fin

dentro_rango:
    MOV R0, #0
    B       fin

fin:
    BX LR
