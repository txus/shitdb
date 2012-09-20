# ShitDB

Shitty key-value store in C inspired by Redis.

## Usage

    $ make
    $ bin/shitdb [port]

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

## Basic types

* Integer: 2834
* String: "yuhuuuu"
* Array: [42,"foo",9999]
* Hashes: (no literal representation)

## Commands

* GET [key]: Returns the content of a key.
* SET [key] [value]: Sets key to a value of any of the basic types.
* QUIT: Ends the connection.

### Array commands

* APUSH [key] [value]: Pushes value to the array located at key.
* APOP [key]: Pops the array located at key and returns the popped value.
* AAT [key] [index]: Gets the array element at a certain index.
* ACOUNT [key]: Returns the number of elements in the array.

### Hash commands

* HSET [key] [hashkey] [value]: Sets the hashkey in the Hash object in key to a value.
* HGET [key] [hashkey]: Gets the value of the hashkey in the Hash object in key.

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
