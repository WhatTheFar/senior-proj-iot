new Vue({
  el: "#vue-app",
  data: {
    numberOfPeople: 0,
    currentNumberOfPeople: 12,
    resetBackgroundTimer: 10,
    resetBtnIsClicked: false,
    resetBtnDisable: false
  },
  methods: {
    setNumberOfPeople: function() {
      if (this.numberOfPeople < 0) {
        alert("Please input valid number" + "\n" + "The number must be equal or more than 0");
      } else {
        console.log("People: " + this.numberOfPeople);
      }

      //   this.$http.post("action.php", this.numberOfPeople, function(data) {
      //
      //   });
    },
    resetNumberOfPeople: function() {
      this.resetBtnIsClicked = true;
      this.resetBtnDisable = true;
      alert("Wait for 10 seconds");
      setTimeout(
        function() {
          this.resetBtnIsClicked = false;
          this.resetBtnDisable = false;
        }.bind(this),
        10000
      );
      //   this.$http.post("action.php", this.numberOfPeople, function(data) {
      //
      //   });
    }
  }
});
