# Setup

Set up [brew](https://brew.sh) on your Mac
```
brew install imagemagick
gem install bundler
bundle install
```

Start up ESP32 with "ImageFromWiFi"

# Usage

```
bundle exec ./send.rb 192.168.x.x image.png
bundle exec ./send.rb 192.168.x.x image.jpg
```

The image height must be larger than its width * 2.

The image will be resized to 64 pixels width and scrolled vertically.
