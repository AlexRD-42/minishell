In the child branch of a fork(), it is normally incorrect to use exit(), 
because that can lead to stdio buffers being flushed twice, and temporary files being unexpectedly removed.

Excerpted from: http://www.unixguide.net/unix/programming/1.1.3.shtml
