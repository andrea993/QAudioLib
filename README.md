# QAudioLib
An audio management library written in Qt

This library allows to manage stereo audio signal, 16bit PCM. It provides `QAudioLibBuffer` that overloads and improve the Qt `QAudioBuffer`, `QAudioLibFreqBuffer` that works with complex number to work in the frequency domain and `Mathematics` class that perform common signal processing operations.

## Default format
Currently moment `QAudioLib` works with an own audio format: `DefaultAudioFormat`:
```C++
setByteOrder(QAudioFormat::LittleEndian);
setChannelCount(2);
setCodec("audio/pcm");
setSampleRate(44100);
setSampleSize(16);
setSampleType(QAudioFormat::SignedInt);
```
Use `QAudioLibBuffer::getDefaultFormat()` to get the default
format.

## QAudioLibBuffer
###Constructors
```C++
QAudioLibBuffer() //a void, no samples buffer
QAudioLibBuffer(const QAudioBuffer & other) //a copy of `other` buffer
QAudioLibBuffer(const QByteArray & data) //a buffer from bytes `data`
QAudioLibBuffer(int numFrames //a zeros buffer with `numFrames` length
QAudioLibBuffer(qreal sec) //a zeros buffer of `sec` seconds length
```

### S16S type
This type is provided by Qt and allows to manage left and right channels in the buffer, for example:
```C++
int position=0;
qint16 val=5;
mybuffer.data()[position].left=val;
mybuffer.data()[position].right=SHRT_MAX;
```
### Data
```C++
const S16S* constData() const //returns an S16S array, read only.
S16S* data() //returns an `S16S array, read and write
```
See also Qt `QAudioBuffer` for inner functions.

### Operators
`QAudioLibBuffer` allows to manipulate buffers through its operators:
```C++
QAudioLibBuffer operator<<(const QAudioLibBuffer &x) const //appends buffers
QAudioLibBuffer operator*(const QAudioLibBuffer &x) const //multiplicates buffers
QAudioLibBuffer operator*(const qreal x) const` //multiplicates for a costant
QAudioLibBuffer operator+(const QAudioLibBuffer &x) const //sums buffers
```

## QAudioLibFreqBuffer
This buffer is really similar to `QAudioLibBuffer`, then I will not explain each function. It works with complex number
### qcomplex type
`qcomplex` manage double complex numbers, to declare a complex number use `qcomplex(re,im)`

### Constructors
```C++
QAudioLibFreqBuffer(int numFrames)
QAudioLibFreqBuffer(const QAudioLibFreqBuffer & other)
static QAudioLibFreqBuffer newByRealBuffer(const QAudioLibBuffer & x) //it is not a constructor but create a complex buffer from a real buffer
```
### S64CD type
It is defined in the library, it is like `S16S` but complex

### Data
```C++
S64CD* data()
const S64CD* constData() const
int frameCount() const
int byteCount() const
void appendZeros(int n) //zero padding, used by FFT
void clear() //clears the buffer
```

### Frequency domain utility
```C++
qreal deltaf() //returns the frequency increment between successive samples
qreal nyquistfreq() //returns the nyquist frequency
QAudioLibFreqBuffer mod() const //returns a new buffer of samples module
QAudioLibFreqBuffer phase() const //returns a new buffer of samples phase
QAudioLibFreqBuffer conj() const //returns a new buffer of samples conjugate
```

### Operators
```C++
QAudioLibFreqBuffer operator*(const QAudioLibFreqBuffer &x)
QAudioLibFreqBuffer operator*(const qreal x)
void operator=(const QAudioLibFreqBuffer &x)
```

## Mathematics
This class provides a list of static functions to generate waveforms and to analyze sounds

### Sound generation
```C++
static QAudioLibBuffer sinwave(qreal f, qreal sec) //generates a new sinusoidal waveform of frequency `f` and duration `sec`
static QAudioLibBuffer squarewave(qreal f, qreal sec) //generates a new sqaure waveform of frequency `f` and duration `sec`
```

### Sound manipulation
```C++
static QAudioLibBuffer conv(QAudioLibBuffer &u ,QAudioLibBuffer &v) //does the convolution of two signals
static QAudioLibBuffer corr(QAudioLibBuffer &u ,QAudioLibBuffer &v) //does the cross correlation of two signals
```

### Fourier transform
```C++
static QAudioLibFreqBuffer fft(const QAudioLibFreqBuffer & x) //does the fft of a complex signal, woks only if the length is a power of 2
static QAudioLibFreqBuffer dft(const QAudioLibFreqBuffer & x) //does the dft, it is very slow (uses at your own risk)
static QAudioLibFreqBuffer fft(const QAudioLibBuffer & x, bool zeropadding=true) //does the dft of a real signal, if signal is a power of 2 or if `zeropadding` is `true` uses the fft alghoritm othermise uses the slow dft. If `zeropadding` is true the signal lenght will change to become a power of 2.
static QAudioLibBuffer ifft(const QAudioLibFreqBuffer & x) //does the inverse dft, uses fft alghorithm only if the input singal is a power of 2 length.
```



