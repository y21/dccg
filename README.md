#### This is not compatible with discordcaptcha v5+
# dccg

This repository contains the source code for dccg, a config-file generator for [DiscordCaptcha](https://github.com/y21/discordcaptcha).<br/>

### Compiling / Linking
It can be compiled on any Linux distribution, Mac OS and Windows and no other external library is required. Make sure these header files are linked while compiling:<br/>
- stdio.h  <br/>
- stdlib.h <br/>
- string.h <br/>
Example compilation:<br/>
```sh
gcc src/main.c -o dccg
```

### How to use
Once compiled, you can execute the compiled source code without any arguments. It should show a very simple menu which expects a number. To start the setup, type `1` and hit return. <br/>

### Terminology
- Bot Token: a key that is used to identify. It's like a password and can be used to login, so treat it like your password. More [here](https://www.writebots.com/discord-bot-token/)<br/>
- Prefix: a key combination that triggers the bot. For instance the command `!ping` would have `!` as prefix<br/>
- Delete messages: whether messages inside of the verification channel should automatically be deleted. It's recommended to turn this on. (1 = enable, 0 = disable)<br/>
- Presence type: this can be either `PLAYING`, `WATCHING` or `LISTENING`. Leave blank if you don't want a presence<br/>
- Presence name: the "game" name that's displayed below the presence type. <br/>
- "To how many servers should discordcaptcha listen to": the number of servers discordcaptcha should react to. You'll be asked for each Server ID afterwards.<br/>
- Command executor: an array ("list") of User IDs that have access to this command. It can be left empty if everyone may execute it.<br/>
- Command permissions: an array ("list") or required permissions for this command. Only users with those permissions may execute it.<br/>
