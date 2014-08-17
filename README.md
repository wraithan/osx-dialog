# osx-dialog

Instead of dynamically writing AppleScript and then piping it into the binary, I
decided to write a native module that uses CoreFoundation to display a dialog.
I'll be iterating on this and adding features I need. If you want something
stable, bind to an exact version. Until 1.0 assume all APIs will break between
versions.

## usage

```JavaScript
var dialog = require('osx-dialog')

dialog({
  title: 'A Title',
  msg: 'some message',
  checkboxes: [
    'first box',
    'second box'
  ]
}, function (button, checkbox_array) {
	// do stuff with the result.
})
```

In the options object `title` and `msg` are required, `checkboxes` is optional.
The second argument, `callback` is required for now but should change in the
future.

## todo

- [ ] Be able to set the buttons.
- [ ] Be able to use radio types.
- [ ] Make callback optional.
