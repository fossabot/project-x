# project-x
This is a small terminal MMO for Windows. It's under development, I add something to this almost daily.

This project uses following external libraries:
* Boost (boost_1_65_1)
* pdcurses (v 1.295 2008/07/15 17:13:25)
* cppconn (MySQL Connector C++ 1.1.9)

Everything else is regular Windows API.

---

# Setup

To setup this project, you need the dependencies I've listed above.
If you have them just go ahead and compile with your favourite compiler.

If compiling succeeds, required database will be automatically created and you can start using the product right away.

# Features

* Complete registration using MySQL.
* Complete security checks during login/registration.
* Checks for duplicate mails/usernames.
* (TBA) Password frequency/security level checker and if password is widely used, or weak prompter.
* Forgotten password service which send you a mail that looks like:

```html
Hey, duman!

Someone (hopefully you!) has submitted a forgotten password request for your account on DumanSTUDIOS projectX

Here's the token to update your password: r68x6

If you didn't do this request just ignore or contact us at https://support.dumanstudios.com
```

* Auto saving (enable/disable) feature which can be set to 15 seconds as the minimum. (Can be modified.)
* In-app password changing.
* AFK DumanBattle™ where you can set your status as online or offline and people can fight you while you're AFK.

_Feature above is set in a way so, even if you shut off your device your last status gets saved and used._
* (TBA) Real-time DumanBattle™ where you can send commands in real time against another person.
* (In-development) Automatically power adjusting AI.

_Monsters gets better as you get better. Program also silently tracks your death counts and if it senses that you're struggling, keeps lowering the amount of the damage dealt by the monsters until 50%._
* An auto healing item that drops randomly from any battle with a chance of 0.0001, adjustable.

_If you somehow get the auto-healing item, in the options menu a new option will be activated. Which you can toggle (on/off) the auto-healing item. If it's set to on, it'll rapidly heal you while you're not in a battle._
* (In-development) Seperate PvP and AFK DumanBattle™ systems so that AI will behave different.

# Notes

* This is my hobby and free-time project, so don't expect me to fix bugs rapidly.
* I keep modifying/adding/removing snippets from the code, time to time I'll update the code here.
* If you're having an issue or if you have a feedback request, please do use [Issues](https://github.com/tkduman/project-x/issues) section.
* Feel free to contribute to the code. I'll review and apply every single bit that improves this project.
* You can find a playable/working version in the [Releases](https://github.com/tkduman/project-x/releases) section.
