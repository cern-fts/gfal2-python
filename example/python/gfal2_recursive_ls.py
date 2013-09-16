#!/usr/bin/env python
import gfal2
import optparse
import stat
import sys
from datetime import datetime


class Crawler:

    def __init__(self, long=False, recursive=False,
                 max_levels=2, context=gfal2.creat_context()):
        self.context = context
        self.long = long
        self.recursive = recursive
        self.max_levels = max_levels

    def _short_format(self, fname):
        return fname

    def _mode2str(self, mode):
        s = ['-'] * 10
        if stat.S_ISDIR(mode):
            s[0] = 'd'
        elif stat.S_ISLNK(mode):
            s[0] = 'l'
        elif not stat.S_ISREG(mode):
            s[0] = '?'

        for i in range(3):
            if mode & stat.S_IRUSR:
                s[1 + i * 3] = 'r'
            if mode & stat.S_IWUSR:
                s[2 + i * 3] = 'w'
            if mode & stat.S_IXUSR:
                s[3 + i * 3] = 'x'
            mode = mode << 3
        return ''.join(s)

    def _long_format(self, fname, fstat):
        return "%s %3d %5d %5d %10d %s %s" % \
            (self._mode2str(fstat.st_mode),
             fstat.st_nlink, fstat.st_uid, fstat.st_gid,
             fstat.st_size,
             datetime.fromtimestamp(fstat.st_mtime).strftime('%b %d %H:%M'),
             fname)

    def _crawl(self, url, out, level=0):
        if level > self.max_levels:
            return

        tabbing = '  ' * level
        try:
            entries = [f for f in self.context.listdir(url)
                       if f != '.' and f != '..']
        except gfal2.GError:
            print >>out, tabbing, '!'
            return

        for f in entries:
            full = url + '/' + f
            # Do the stat only if we need to
            if self.recursive or self.long:
                try:
                    fstat = self.context.stat(full)
                except gfal2.GError:
                    fstat = self.context.st_stat()

            # Print entry
            if self.long:
                print >>out, tabbing, self._long_format(f, fstat)
            else:
                print >>out, tabbing, self._short_format(f)

            # Descend
            if self.recursive and stat.S_ISDIR(fstat.st_mode):
                self._crawl(full, out, level + 1)

    def crawl(self, url, out=sys.stdout):
        self._crawl(url, out)


if __name__ == '__main__':
    # Get the options
    parser = optparse.OptionParser()
    parser.add_option('-l', '--long', dest='long',
                      default=False, action='store_true',
                      help='Long format')
    parser.add_option('-r', '--recursive', dest='recursive',
                      default=False, action='store_true',
                      help='Recursive')
    parser.add_option('-m', '--max', dest='max_recursive',
                      default=1, type='int',
                      help='Maximum recursive level')

    (options, args) = parser.parse_args()
    if  len(args) != 1:
        parser.error('Incorrect number of arguments. Need a path')

    url = args[0]

    # List recursively
    crawler = Crawler(options.long, options.recursive, options.max_recursive)
    crawler.crawl(url, sys.stdout)
