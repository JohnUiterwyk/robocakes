Markdown  				{#markdown}
========

This is actually a Markdown tutorial. This is a paragraph. Just type.
Linebreaks are ignored, and the parser does it's own wrapping, so to start
a new paragraph, insert a blank line.

Like this, see?

Section headers are easy too. If you just need two levels of header, you can
use the following:

H1 style header
===============

H2 style header
---------------

Or, if you need to nest deeper, you can go on with hashes

# This is a level 1 header

### This is level 3 header #######

The number of # at the end of the line don't matter

You can go as deep as 6

###### header 6

Block quotes can be put in using >

From stack overflow:
> After some investigation, it seems Doxygen currently does not support the
> ordering of pages in a custom (or any) fashion.

To make a list, just draw them with \*, - or +

* Item 1
  More text for this item can be added, so long as it is indented the same
  way as the text above.
* Item 2
  * Subitem.
  * And another subitem.

A numbered list:

1. This is the story
2. Of our first teacher
3. Shetland made her jumpers
4. And the devil made her features.

Code blocks (might be important?) can be added with a blank line and some
indentation (which will get removed)

    def foo(bar):
    	print bar

And that's the end of the code.

*To provide* em-fah-sis _on a_ sill-a-bul, use a star or an underscore

To distract people **with bold text**, use two stars of __two__ underscores

Inline code sections, like `printf()`, can be marked with backticks (\`)

Links (probably important too) can be written up like the following:

[the link text](http://the-link-target.com.org.foo)

If you want to embed links in text, you can use their name in square brackets
like [stack overflow] and then define it somewhere else in the text using the
`[link name]:link url` syntax

[stack overflow]:http://stackoverflow.com

There is other stuff that is added on to markdown and I don't know about: it's
documented on the [doxygen markdown manual] - the syntax highlighting looks
nice. Tables too, which are always a pain.

[doxygen markdown manual]:http://www.stack.nl/~dimitri/doxygen/manual/markdown.html
