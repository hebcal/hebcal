BINARY_NAME=hebcal
MAN1_NAME=$(BINARY_NAME).1
PREFIX=/usr/local
MANDIR=/share/man

all: $(BINARY_NAME) $(MAN1_NAME)

$(BINARY_NAME): main.go gnu.go user.go version.go
	go build -o $(BINARY_NAME)

.PHONY: clean
clean:
	go clean
	rm -f $(BINARY_NAME)
	rm -f $(MAN1_NAME)

check test: all
	go test ./...

man: $(MAN1_NAME)

$(MAN1_NAME): $(MAN1_NAME).in version.go
	$(eval VERSION := $(shell grep Version version.go | sed -e 's/.*"\([0-9]*\.[0-9]*\.[0-9]*\)"/\1/'))
	sed -e 's/@VERSION@/$(VERSION)/' $(MAN1_NAME).in > $(MAN1_NAME)

.PHONY: install
install: $(BINARY_NAME) $(MAN1_NAME)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $(BINARY_NAME) $(DESTDIR)$(PREFIX)/bin/$(BINARY_NAME)
	mkdir -p $(DESTDIR)$(PREFIX)$(MANDIR)/man1
	cp $(MAN1_NAME) $(DESTDIR)$(PREFIX)$(MANDIR)/man1/$(MAN1_NAME)

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BINARY_NAME)

