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
      console.log(this.numberOfPeople);
      //   this.$http.post("action.php", this.numberOfPeople, function(data) {
      //
      //   });
    },
    resetNumberOfPeople: function() {
      //   this.$http.post("action.php", this.numberOfPeople, function(data) {
      //
      //   });
    }
  }
});
