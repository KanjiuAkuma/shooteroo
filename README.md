Shooteroo
---

Shooteroo is simple dodge game using OpenGL written in C++.

Developed by Kami-Kaze (@KanjiuAkuma)

### Objective
Survive as long as you can.

Flying projectiles will kill you if the hit you.  
Monsters will also kill you if they reach you, but you can kill those.


### Abilities
**Shoot (`Q`):**
> Shoot a bullet, that kills hostiles, towards the cursor.

**Boost (`W`):**
> Gain increased movement speed boost for a shot time.

**Blink (`E`):**
> Blink a short distance towards the cursor.

**Ult (`R`):**
> Turn invulnerable to projectiles  
> gain increased movement speed and  
> reduce cooldowns of other abilities  
> for a short time.

### Inputs
__Mouse__
>   `Left click`: (Re-) start the game  
>   `Right click`: Move

__Keyboard__
>   `Q`: Shoot  
>   `W`: Boost  
>   `E`: Blink  
>   `R`: Ult  
>   `S`: stop the player  
>   `H`: show this window  
>   `O`: open the option window  
>   `Escape`: close the game


###  Releases
__Debug__:  
Logs output to the console,   
Has padding around game content view,  
Shows settings by default,  
Does not spawn projectiles and hostiles by default.  

__Release__:  
No console.

### Mentions
A lot of the engine code, is based of Hazel engine by @TheCherno and modified to fit my needs.