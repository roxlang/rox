ROX v0 — string Specification (Final Minimal)
1️⃣ Type
string

Built-in object type

Immutable

Represents a sequence of bytes (UTF-8 by convention)

String literals produce string

Example:

string s = "Hello ROX!\n";

2️⃣ Immutability

Illegal:

s.at(0) = 'H' ❌
s.append('!') ❌

There are no mutation APIs.

3️⃣ Methods

Only:

size() -> num
at(i) -> rox_result[char]
toList() -> list[char]

Example:

num n = s.size();
rox_result[char] r = s.at(0);
list[char] xs = s.toList();

# 4️⃣ Equality

!=

Defined for all objects

For string: value equality

Same length + same byte sequence

Example:

if (s == "abc") {
...
}

5️⃣ No Reverse Conversion

There is no:

stringFromList

If a user wants to print characters:

list[char] xs = ...;

repeat i in range(0, xs.size(), 1) {
rox_result[char] r = xs.at(i);
if (not isOk(r)) { return none; }
printChar(getValue(r));
}

You keep:

Explicit control

No hidden encoding assumptions

No implicit text construction

Design Philosophy Preserved

Immutable text

Mutable list

Explicit conversion only one way

No builders

No hidden coercion

One way
