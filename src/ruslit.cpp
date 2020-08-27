﻿#include "ruslit.h"

/*
Контейнер unicode Значений для используемых символов

*/
void fill_symbols(std::map<std::string,int>& rus_lit){

         rus_lit.insert({u8"A", 0x0041});
         rus_lit.insert({u8"B", 0x0042});  
         rus_lit.insert({u8"C", 0x0043});  
         rus_lit.insert({u8"D", 0x0044});
         rus_lit.insert({u8"E", 0x0045});
         rus_lit.insert({u8"F", 0x0046});  
         rus_lit.insert({u8"G", 0x0047});  
         rus_lit.insert({u8"H", 0x0048});        
         rus_lit.insert({u8"I", 0x0049});
         rus_lit.insert({u8"J", 0x004A});  
         rus_lit.insert({u8"K", 0x004B});  
         rus_lit.insert({u8"L", 0x004C});        
         rus_lit.insert({u8"M", 0x004D});
         rus_lit.insert({u8"N", 0x004E});  
         rus_lit.insert({u8"O", 0x004F});  
         rus_lit.insert({u8"P", 0x0050});        
         rus_lit.insert({u8"Q", 0x0051});
         rus_lit.insert({u8"R", 0x0052});  
         rus_lit.insert({u8"S", 0x0053});  
         rus_lit.insert({u8"T", 0x0054});        
         rus_lit.insert({u8"U", 0x0055});
         rus_lit.insert({u8"V", 0x0056});  
         rus_lit.insert({u8"W", 0x0057});  
         rus_lit.insert({u8"X", 0x0058});
         rus_lit.insert({u8"Y", 0x0059});  
         rus_lit.insert({u8"Z", 0x005A});    
                      
//        
         rus_lit.insert({u8"0", 0x0030});
         rus_lit.insert({u8"1", 0x0031});
         rus_lit.insert({u8"2", 0x0032}); 
         rus_lit.insert({u8"3", 0x0033});
         rus_lit.insert({u8"4", 0x0034});
         rus_lit.insert({u8"5", 0x0035});
         rus_lit.insert({u8"6", 0x0036});
         rus_lit.insert({u8"7", 0x0037});
         rus_lit.insert({u8"8", 0x0038});
         rus_lit.insert({u8"9", 0x0039});

         rus_lit.insert({u8",", 0x002C});   
         rus_lit.insert({u8" ", 0x0020});
         rus_lit.insert({u8":", 0x003A});
         rus_lit.insert({u8"!", 0x0021});
         rus_lit.insert({u8"\"", 0x0022});
//Russian symbols
         rus_lit.insert({u8"А", 0x0410});
         rus_lit.insert({u8"Б", 0x0411});
         rus_lit.insert({u8"В", 0x0412});
         rus_lit.insert({u8"Г", 0x0413});
         rus_lit.insert({u8"Д", 0x0414});
         rus_lit.insert({u8"Е", 0x0415});
         rus_lit.insert({u8"Ё", 0x0401});
         rus_lit.insert({u8"Ж", 0x0416});
         rus_lit.insert({u8"З", 0x0417});
         rus_lit.insert({u8"И", 0x0418});
         rus_lit.insert({u8"Й", 0x0419});
         rus_lit.insert({u8"К", 0x041A});
         rus_lit.insert({u8"Л", 0x041B});
         rus_lit.insert({u8"М", 0x041C});
         rus_lit.insert({u8"Н", 0x041D});
         rus_lit.insert({u8"О", 0x041E});
         rus_lit.insert({u8"П", 0x041F});
         rus_lit.insert({u8"Р", 0x0420});
         rus_lit.insert({u8"С", 0x0421});
         rus_lit.insert({u8"Т", 0x0422});
         rus_lit.insert({u8"У", 0x0423});
         rus_lit.insert({u8"Ф", 0x0424});
         rus_lit.insert({u8"Х", 0x0425});
         rus_lit.insert({u8"Ц", 0x0426});
         rus_lit.insert({u8"Ч", 0x0427});
         rus_lit.insert({u8"Ш", 0x0428});
         rus_lit.insert({u8"Щ", 0x0429});
         rus_lit.insert({u8"Ъ", 0x042A});
         rus_lit.insert({u8"Ы", 0x042B});
         rus_lit.insert({u8"Ь", 0x042C});
         rus_lit.insert({u8"Э", 0x042D});
         rus_lit.insert({u8"Ю", 0x042E});
         rus_lit.insert({u8"Я", 0x042F});
         rus_lit.insert({u8"а", 0x0430});
         rus_lit.insert({u8"б", 0x0431});
         rus_lit.insert({u8"в", 0x0432});
         rus_lit.insert({u8"г", 0x0433});
         rus_lit.insert({u8"д", 0x0434});
         rus_lit.insert({u8"е", 0x0435});
         rus_lit.insert({u8"ё", 0x0451});
         rus_lit.insert({u8"ж", 0x0436});
         rus_lit.insert({u8"з", 0x0437});
         rus_lit.insert({u8"и", 0x0438});
         rus_lit.insert({u8"й", 0x0439});
         rus_lit.insert({u8"к", 0x043A});
         rus_lit.insert({u8"л", 0x043B});
         rus_lit.insert({u8"м", 0x043C});
         rus_lit.insert({u8"н", 0x043D});
         rus_lit.insert({u8"о", 0x043E});
         rus_lit.insert({u8"п", 0x043F});
         rus_lit.insert({u8"р", 0x0440});
         rus_lit.insert({u8"с", 0x0441});
         rus_lit.insert({u8"т", 0x0442});
         rus_lit.insert({u8"у", 0x0443});
         rus_lit.insert({u8"ф", 0x0444});
         rus_lit.insert({u8"х", 0x0445});
         rus_lit.insert({u8"ц", 0x0446});
         rus_lit.insert({u8"ч", 0x0447});
         rus_lit.insert({u8"ш", 0x0448});
         rus_lit.insert({u8"щ", 0x0449});
         rus_lit.insert({u8"ъ", 0x044A});
         rus_lit.insert({u8"ы", 0x044B});
         rus_lit.insert({u8"ь", 0x044C});
         rus_lit.insert({u8"э", 0x044D});
         rus_lit.insert({u8"ю", 0x044E});
         rus_lit.insert({u8"я", 0x044F});
}

