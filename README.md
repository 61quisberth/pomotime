## PomoTime - Tomato-based productivity timer

[Appstore link](https://apps.getpebble.com/en_US/application/5738d37c771f1fc4ed000017)

Sleek, simple, and intuitive pomodoro timer for use with Pebble.
Spend less time figuring out how to use the app and more time collecting pomodori!
 
Features three modes: pomodoro (25 min), short break (5 min), and long break (15 min).
Each mode allows one to pause, play, or reset timer.
A vibration notification tells the user that time is up.
 
The pomodoro (Italian for “tomato”) technique is a productivity technique involving
a 25 minute segment of concentrated work, followed by some short or long break.
See http://pomodorotechnique.com/ for more details on the technique.
 
Tested and developed with the Pebble Time family in mind.
Ever free and open source (MIT license). Developer is always
welcoming comments/suggestions/patches for future versions.

### Using pebble cli
building
  - release
    - pebble build
  - debug
    - CFLAGS=-DDEBUG pebble build

running on emulator
  - pebble install --emulator [platform]
 
Tags: pomodoro, pomodori, productivity, time management, timer

### pebble docs
c sdk docs
https://developer.pebble.com/docs/c/

newlib ansi c lib that pebble uses as its std
https://sourceware.org/newlib/

### release notes
  - v1.0
    - First release of productivity timer usable with Aplite, Basalt, and Chalk platforms
  - v2.0
    - Replaced short-duration double pulse notifications with longer duration vibration notification
      - similar to the notification experienced with pebble alarms
