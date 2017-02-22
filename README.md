# FlightGo
A simple framework for building a flight war game(Based on MFC)
  
  
### Main Design
>  Split the concept of 'object' and 'action'(See ***FlightObject.h*** and ***Actable.h*** ), so that 
you can define an action(In this framework it's the ***actor*** class) for different kinds of objects.
For example, you can use the same 'shoot bullet' actor for you hero flight or the enemy flights. It's 
very useful to use this desgin when you want to change your flight objects' action dynamically.  
>  
Use message driven design to simplify the interaction between flight objects. In flightgo, every flight object 
is listenable. So that you can register callbacks to control you flights' action. For example, register a collapse 
action to you hero flight so that when a collison occures between your hero flight and enemy bullet, your flight will just 
eliminate itself.  
>    
### Collision
>  In flightgo, flights' border can be represented by pollygen region, circle or ellipse. You can define your flight's border in ***config.ini*** and use it in your program. The collison occurs when lines cut across with each other, and the relative objects 
would receive a message so that the registered callbacks will be called to control the flights' action.
>  
### Time, Resource and Keyboard management
>  In flightgo, we use TimerManager and Timer to perform delayed call or call functions periodically. To manage resources such as 
images we use ResourceManager. To monitor keyboard event and register callbacks, we can use KeyboardManager.
