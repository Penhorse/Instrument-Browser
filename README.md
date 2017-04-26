# Instrument Browser

Reaktor instrument browser, designed for working with Reaktor 6 blocks. It can be used to browse regular instruments too as long as they meet the requirements (see info below). Only works with instruments generated with Reaktor 6 and up.

![ibrows 1 3 1](https://cloud.githubusercontent.com/assets/2492870/11008113/6352fb58-84c6-11e5-9803-f246b06846bd.png)

## Information for block developers

* If your block shows up without an icon it's because you didn't add a panel icon to the main instrument module.

* Blocks must have a background image otherwise they won't show up at all. This is because of the way the \*.ism files are parsed - we search the file byte by byte for recognisable patterns of data and the background image serves as a handy anchor from which the rest of the information is derived. For implementation details see [ismsnoop](https://github.com/Penhorse/ismsnoop).

* If your block has a background image and still doesn't show up, it's probably a problem with the \*.ism parser. Please submit an issue!

* Usually when NI releases a new version of Reaktor any new blocks created with that version will fail to parse, because the format of the \*.ism files changes slightly. In this case an update to the \*.ism parser is required. Please bug me until I do it (or do it yourself and submit a pull request ;) )
