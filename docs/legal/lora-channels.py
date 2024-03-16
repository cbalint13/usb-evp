#!/usr/bin/python3

import sys
import numpy as np
import matplotlib.pyplot as plt


debug = True

# ism-licenses-eu.txt
bk = [(863000, 865000, 25, 0.1, None),
      (865000, 868000, 25, 1.0, None),
      (868000, 868600, 25, 1.0, None),
      (868600, 868700, 10, 0.1,   25),
      (868700, 869200, 25, 0.1, None),
      (869200, 869250, 10, 1.0,   25),
      (869250, 869300, 10, 1.0,   25),
      (869300, 869400, 10, 1.0,   25),
      (869400, 869650, 500, 10, None),
      (869650, 869700, 25,  10,   25),
      (869700, 870000, 25, 1.0,   25)]

# LoRA channels width
ch = [7.8, 10.4, 15.6, 20.8, 31.25, 41.7, 62.5, 125, 250, 500]


def get_cmap(n, name='hsv'):
    '''Returns a function that maps each index in 0, 1, ..., n-1 to a distinct 
    RGB color; the keyword argument name must be a standard mpl colormap name.'''
    return plt.cm.get_cmap(name, n)


def annotation_line( ax, xmin, xmax, y, text, ytext=0, linecolor='black', linewidth=1, fontsize=12 ):

    ax.annotate('', xy=(xmin, y), xytext=(xmax, y), xycoords='data', textcoords='data',
            arrowprops={'arrowstyle': '|-|', 'color':linecolor, 'linewidth':linewidth})
    ax.annotate('', xy=(xmin, y), xytext=(xmax, y), xycoords='data', textcoords='data',
            arrowprops={'arrowstyle': '<->', 'color':linecolor, 'linewidth':linewidth})

    xcenter = xmin + (xmax-xmin)/2
    if ytext==0:
        ytext = y + ( ax.get_ylim()[1] - ax.get_ylim()[0] ) / 20

    ax.annotate( text, xy=(xcenter,ytext), ha='center', va='center', fontsize=fontsize)


def merge_adj_blk(blocks):

    while(True):

        i = 0
        nblocks = []
        l = len(blocks)
        while(i < len(blocks)):

            # last block
            if (i == len(blocks)-1):
                nblocks.append(blocks[i])
                break

            # merge continuous adjacent
            if ((blocks[i][1] == blocks[i+1][0])
             and(blocks[i][2] == blocks[i+1][2])
             and(blocks[i][3] == blocks[i+1][3])):
                nblocks.append((blocks[i][0], blocks[i+1][1], blocks[i][2], blocks[i][3]))
                i += 2
                continue
            else:
                nblocks.append(blocks[i])

            i += 1
        if (l == len(nblocks)):
            return nblocks

        blocks = nblocks


def main():

    global bk
    y_anno_pos = 120
    cmap = get_cmap(32)
    bk = merge_adj_blk(bk)

    fig, ax = plt.subplots( figsize = ( 20, 10 ), dpi=100 )
    ax.set_ylim(300, 0)

    for b in bk:

        y = 0
        ax.axvline(b[0]/1000)

        for c in ch:

            s = b[0] / 1000
            e = b[1] / 1000
            c = c / 1000

            if debug:
                print ("CH band %.2f kHz" % c)

            for i in range(0,2000):

                if ( s+c > e ):
                    break

                if debug:
                    print ("    CH %02i %0.2f - %0.2f kHz [%i %i]" % (i,s,s+c, y, y+10))
                ax.broken_barh([(s,c)], (y, 10), facecolors=cmap(i), alpha=0.5, linewidth=0.5, edgecolor='k')

                s += c

            y += 10

        annotation_line( ax=ax, text="%.2f - %.2f MHz\n(%i mW) %.1f%%" % (b[0]/1000, b[1]/1000, b[2], b[3]), xmin=b[0]/1000, xmax=b[1]/1000, y=y_anno_pos, ytext=y_anno_pos+10, linewidth=2, linecolor='red', fontsize=8)
        y_anno_pos += 20

    ax.axvline(b[1]/1000)

    plt.savefig("lora-channels-EU.svg", format='svg', bbox_inches='tight')
    plt.savefig("lora-channels-EU.png", format='png', bbox_inches='tight')

    plt.show()
    plt.close()


if __name__ == '__main__':
    main()
