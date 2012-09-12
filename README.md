# ShitDB

Shitty key-value store in C inspired by Redis.

## Usage

    $ make
    $ bin/shitdb

Example session:

    $ nc localhost 4999
    SET foo "bar"
    OK
    GET foo
    "bar"
    SET bar 123
    OK
    GET bar
    123
    QUIT
    Bye!

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Added some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request

## Who's this

This was made by [Josep M. Bach (Txus)](http://txustice.me) under the MIT
license. I'm [@txustice](http://twitter.com/txustice) on twitter (where you
should probably follow me!).
