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
  timeout: 10,
  checkboxes: [
    'first box',
    'second box'
  ]
}, function (error, button, checkboxes) {
	// do stuff with the result.
})
```

###`dialog(options, [callback])`

* **`options`** (required) has the following keys
  * **`title`** (required) is the bolded portion of the dialog
  * **`message`** (required) is the text area portion of the dialog
  * **`timeout`** (optional) is the duration the dialog should be displayed before it is dismissed
  * **`checkboxes`** (optional) is an array of strings that will be displayed as checkboxes
* **`callback`** (optional) has the signature callback(error, button, checkboxes)
  * **`error`** will be `null` or a string with the error message
  * **`button`** will be a boolean noting if it was dismissed by hitting the button (`true`) or dismissed by hitting escape (`false`)
  * **`checkboxes`** will be an array of booleans with the status of the checkboxes in the order that was given to `dialog`

## todo

- [ ] Be able to set the buttons.
- [ ] Be able to use radio types.
