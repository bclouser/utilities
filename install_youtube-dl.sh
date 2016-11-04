#!/usr/bin/env bash

which brew || {
	echo "Homebrew not installed. We will do that first."
	xcode-select --install || {
		echo "Grrr, i don't like xcode. Stopping"
		return -1
	}
	ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)" || {
		echo ""
		echo "Bummer. Installing homebrew didn't go so well, or perhaps it is already installed. Stopping here"
		exit -1
	}
}


echo "Yay, homebrew is installed!!!"

if which youtube-dl ;then
	echo "youtube-dl is already installed... not installing again"
else
	echo ""
	echo "Alright, installing youtube-dl"
	brew install youtube-dl || {
		echo "ugh, something here went wrong during our install of youtube-dl"
		exit -1
	}
fi

if which ffmpeg ; then
	echo "ffmpeg is already installed. not installing again"
else
	echo ""
	echo "So far so good, lastly install ffmpeg3"
	brew install ffmpeg || {
		echo "Blast! installing ffmpeg3 failed"
		return -1
	}
fi

echo ""
echo "well, it looks like this worked"