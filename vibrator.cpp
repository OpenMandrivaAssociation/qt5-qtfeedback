/* Example and test to drive the PinePhone vibrator
 * Detects the vibrator device and starts a 250ms rumble
 * (should be similar to keyboard haptic effect)
 *
 * Not actually used; kept in the package as a reference/test
 * for further developing the evdev driver.
 *
 * (C) 2020 Bernhard Rosenkränzer <bero@lindev.ch>
 *
 * Released under the GPLv3
 */
#include <QDir>
#include <QFile>
#include <iostream>
#include <cstdint>

extern "C" {
#include <fcntl.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <unistd.h>
}

template<size_t s> class BitField {
public:
	BitField() { memset(data, 0, sizeof(data)); }
	operator void*() { return static_cast<void*>(&data); }
	bool isSet(int bit) const {
		return (data[(bit/8)]>>(bit%8))&1;
	}
	void set(int bit) {
		data[(bit/8)] |= 1<<(bit%8);
	}
	void clear(int bit) {
		data[(bit/8)] &= ~(1<<(bit%8));
	}
	bool operator[](int bit) const {
		return isSet(bit);
	}
private:
	uint8_t data[1+s/8];
};

int main(int argc, char **argv) {
	QDir devinput("/dev/input");
	for(QString const &dev : devinput.entryList(QStringList() << "event*", QDir::Readable|QDir::Writable|QDir::System|QDir::NoDotAndDotDot)) {
		QString d("/dev/input/" + dev);
		int fd = open(QFile::encodeName(d), O_RDWR);
		if(fd < 0)
			continue;

		std::cerr << qPrintable(dev) << std::endl;

		int effects;
		if(ioctl(fd, EVIOCGEFFECTS, &effects) < 0) {
			perror("EVIOCGEFFECTS");
			close(fd);
			continue;
		}
		std::cerr << effects << " effects supported" << std::endl;

		if(effects <= 0)
			continue;

		BitField<FF_MAX> ffFeatures;
		if(ioctl(fd, EVIOCGBIT(EV_FF, sizeof(ffFeatures)), &ffFeatures) < 0) {
			perror("EV_FF");
			close(fd);
			continue;
		}
		if(ffFeatures.isSet(FF_CONSTANT))
			std::cerr << "constant FF supported" << std::endl;
		if(ffFeatures.isSet(FF_PERIODIC))
			std::cerr << "periodic FF supported" << std::endl;
		if(ffFeatures.isSet(FF_RAMP))
			std::cerr << "ramp FF supported" << std::endl;
		if(ffFeatures.isSet(FF_SPRING))
			std::cerr << "spring FF supported" << std::endl;
		if(ffFeatures.isSet(FF_FRICTION))
			std::cerr << "friction FF supported" << std::endl;
		if(ffFeatures.isSet(FF_DAMPER))
			std::cerr << "damper FF supported" << std::endl;
		if(ffFeatures.isSet(FF_RUMBLE))
			std::cerr << "rumble FF supported" << std::endl;
		if(ffFeatures.isSet(FF_INERTIA))
			std::cerr << "inertia FF supported" << std::endl;
		if(ffFeatures.isSet(FF_GAIN))
			std::cerr << "gain FF supported" << std::endl;
		if(ffFeatures.isSet(FF_AUTOCENTER))
			std::cerr << "autocenter supported" << std::endl;

		if(ffFeatures.isSet(FF_SQUARE))
			std::cerr << "Square FF supported" << std::endl;
		if(ffFeatures.isSet(FF_TRIANGLE))
			std::cerr << "Triangle FF supported" << std::endl;
		if(ffFeatures.isSet(FF_SINE))
			std::cerr << "Sine FF supported" << std::endl;
		if(ffFeatures.isSet(FF_SAW_UP))
			std::cerr << "Saw up FF supported" << std::endl;
		if(ffFeatures.isSet(FF_SAW_DOWN))
			std::cerr << "Saw down FF supported" << std::endl;
		if(ffFeatures.isSet(FF_CUSTOM))
			std::cerr << "Custom FF supported" << std::endl;

		if(ffFeatures.isSet(FF_GAIN))
			std::cerr << "FF Gain supported" << std::endl;

		if(ffFeatures.isSet(FF_GAIN)) {
			// set gain to 75%
			input_event gain;
			memset(&gain, 0, sizeof(gain));
			gain.type = EV_FF;
			gain.code = FF_GAIN;
			gain.value = 0xc000;
			if(write(fd, &gain, sizeof(gain)) != sizeof(gain)) {
				perror("Set gain");
				close(fd);
				continue;
			}
			puts("Gain set to 50%");
		}
		
		ff_effect effecttypes[1];
		effecttypes[0].type = FF_RUMBLE;
		effecttypes[0].id = -1;
		effecttypes[0].u.rumble.strong_magnitude = 0xffff;
		effecttypes[0].u.rumble.weak_magnitude = 0x0;
		effecttypes[0].replay.length = 250;
		effecttypes[0].replay.delay = 0;
		ioctl(fd, EVIOCSFF, &effecttypes[0]);

		input_event start;
		memset(&start, 0, sizeof(input_event));
		start.type = EV_FF;
		start.code = effecttypes[0].id;
		start.value = 1;
		puts("Starting vibrator");
		if(write(fd, &start, sizeof(start)) < sizeof(start)) {
			perror("start vibrator");
			close(fd);
			continue;
		}

		sleep(2);
		input_event stop;
		memset(&stop, 0, sizeof(stop));
		stop.type = EV_FF;
		stop.code = effecttypes[0].id;
		stop.value = 0;
		puts("Stopping vibrator");
		if(write(fd, &start, sizeof(start)) < sizeof(start)) {
			perror("stop vibrator");
			close(fd);
			continue;
		}
		close(fd);
	}
}
