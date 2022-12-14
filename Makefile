BINARY_NAME=hebcal
PREFIX=/usr/local

$(BINARY_NAME): main.go gnu.go user.go version.go
	go build -o $(BINARY_NAME)

all: $(BINARY_NAME)

.PHONY: clean
clean:
	go clean
	rm -f $(BINARY_NAME)

check test: all
	go test ./...

.PHONY: install
install: $(BINARY_NAME)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/$(BINARY_NAME)

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BINARY_NAME)
