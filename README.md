# Dom

Dom is a new and weird way to create webservers with a very basic language.

To get started, install `dom` like so:

```
git clone https://github.com/japrozs/dom
make
./out/dom
```

Then create a basic file like in [/samples/basic.dm](/samples/basic.dm)

the syntax is as follows

```
"<route>" -> "command to be executed"
```

Then `dom` executes the command provided whenever you access that route. the webserver is started at `http://localhost:3000` by default.
