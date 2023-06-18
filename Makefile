BINARY_NAME=hebcal
MAN1_NAME=$(BINARY_NAME).1
PREFIX=/usr/local
MANDIR=/share/man
DEFAULT_CITY="New York"

all: $(BINARY_NAME) $(MAN1_NAME)

$(BINARY_NAME): main.go gnu.go user.go version.go dcity.go
	go build -o $@

.PHONY: clean
clean:
	go clean
	rm -f $(BINARY_NAME) $(MAN1_NAME) dcity.go

.PHONY: check
.PHONY: test
check test: all
	go test ./...

man: $(MAN1_NAME)

dcity.go:
	echo "package main" > $@
	echo "var defaultCity = \"$(DEFAULT_CITY)\"" >> $@

$(MAN1_NAME): $(MAN1_NAME).in version.go
	$(eval VERSION := $(shell grep Version version.go | sed -e 's/.*"\([0-9]*\.[0-9]*\.[0-9]*\)"/\1/'))
	sed -e 's/@VERSION@/$(VERSION)/' $(MAN1_NAME).in > $@

.PHONY: install
install: $(BINARY_NAME) $(MAN1_NAME)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -s $(BINARY_NAME) $(DESTDIR)$(PREFIX)/bin/$(BINARY_NAME)
	mkdir -p $(DESTDIR)$(PREFIX)$(MANDIR)/man1
	cp $(MAN1_NAME) $(DESTDIR)$(PREFIX)$(MANDIR)/man1/$(MAN1_NAME)

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BINARY_NAME)
	rm -f $(DESTDIR)$(PREFIX)$(MANDIR)/man1/$(MAN1_NAME)
