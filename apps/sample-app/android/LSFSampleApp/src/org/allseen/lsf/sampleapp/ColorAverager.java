/*
 * Copyright (c) 2014, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
package org.allseen.lsf.sampleapp;

public class ColorAverager {

    protected long sum;
    protected long count;
    protected long initial;
    protected boolean uniform;

    public ColorAverager() {
        super();
        reset();
    }

    public void reset() {
        sum = 0;
        count = 0;
        initial = -1;
        uniform = true;
    }

    public void add(long value) {
        sum += value;
        count++;

        if (initial < 0) {
            initial = value;
        } else if (uniform) {
            uniform = (value == initial);
        }
    }

    public long getSum() {
        return sum;
    }

    public long getCount() {
        return count;
    }

    public long getAverage() {
        double divisor = count > 0 ? count : 1.0;

        return Math.round(sum / divisor);
    }

    public boolean isUniform() {
        return uniform;
    }
}

