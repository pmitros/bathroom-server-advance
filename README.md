Bathroom Server Advance
====

My undergraduate dorm had just two bathrooms per floor of 12
students. As you might imagine, they were both often full. People
would wander the halls searching for empty bathrooms. Students
installed Hall Effect sensors on the doors, so we could check which
was open and closed online. This was a more advanced version of the
server, which visualized a path to the nearest empty bathroom.

To get this back up and running, we'd need to repopulate bs.db, which
was a database mapping IPs to dorm room numbers. And we'd need to go
back to the days of CGI scripts.

Directory structure: 

/bathroom contains the images and html files that go in the /bathroom
directory on my web server. Might make sense to rename this on the
normal server at some point, but it really doesn't matter.

/cgi-bin contains the obvious. 

/src/client contains the client source. Compile with: 
  gcc *.c

/src/data contains things needed to recreate the data files. moira.pl
extracts info from moira (could be better written using stella, about
which I found out later). This gets all the info we *MAY* need, since
I didn't want to hose the moira DB if we decided to add something else
later. pt.pl cleans it up into the format we want. bath*.tif are the
raw images of a Random Hall with and without all the
connections. cutter.pl breaks it up into the appropriate
files. output-ascii.c is if you ever want to add this into the text
mode version :)

Kludge zones, TODOs, etc.: 

* At the moment, the CGI script counts on the bathroom server to
  respond. This should not be a problem when they are both on the same
  machine. The way I (should have) dealt with it when running on my
  machine was to configure Apache to have a time limit waiting for CGI
  scripts (this is apparently an option somewhere), or to write better
  code :)

* There's a whole bunch of white .gif and green .gifs. These could be
  replaced with a single 1x1 white and a single 1x1 green gif, and
  loaded using height and width tags. Several of the other .gifs are
  probably also identical. Several could be combined. Some could be 
  replaced with &nbsp; and just give a background color for the cell 
  and height/width tags for the cell. Again, could be cleaned up. All
  this would speed up loading. 

* Should have a way to set a cookie to mark if you're in the wrong
  room. Pity that. 

* Ideally, the whole server should have a cookie to select which
  interface you go to by default.

* Technology section should at some point document ports on which to
  interface to the server and stuff like that. Pity that. 

* I should add a [help] section that documents what all the icons
  mean.

* Better graphics. 

Feel free to modify anything as you see fit. 

Enjoy,
- Piotr