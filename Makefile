ENV=dtw

build_and_upload:
	pio run --target upload -e $(ENV)
	
build:
	pio run -e $(ENV)

