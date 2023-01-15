# Tribal Knowledge

## Unknown Stations

There are at least 2 Unknown StopIDs: `R60` and `R65`. These show up in the feed itself, but are not present in `stops.txt`. 

* `R60`

  * Is the location for Northbound R and N trains between Lexington/59th Street and Queens Plaza, which is in the tunnel/elevated Queensboro bridge section of track. 
  * The MTA's Google Group on this has had discussion as long ago as 2013, when this stop was removed from `stops.txt` and `stop_times.txt` (approximatel)
  * https://groups.google.com/g/mtadeveloperresources/search?q=R60
  * It is known as the "11th Street Cut", or "60th Street Tunnel Connection", and is the result of connecting 2 different (original) agencies lines back when the subway was consolidated: https://en.wikipedia.org/wiki/60th\_Street\_Tunnel\_Connection (BMT to IND)

* `R65`

  * Is the location for Southbound R trains between Whitehall and Court Street, which is the tunnel to Brooklyn
  * There is no discussion on this StopId on the Google Group, but I'd imagine it's either a deprecated station or similar connection between two former agencies in the same way as R60. 
 
I'll likely either ignore these, or add an extension to `stops.txt` for my own purposes because even if neither is a station, it's interesting data. I'd guess the time to cross the tunnel is a few minutes more than most intra-station times anyway, so could be another piece of data to add in, even if questionably reliable.

## The `H` line

* The H line was an earlier (not the only, and not the earliest) designation for what is now known as the Rockaway Park Shuttle, out in Queens. It's still listed as the H line in the data, even though most people would recognize it only by the S with a superscript R, like S^R. The superscript distinguishes it from the plain S line, which is the Grand Central Shuttle, and the S^F line, which is the Franklin Avenue Shuttle.
* More here: https://en.wikipedia.org/wiki/Rockaway_Park_Shuttle

