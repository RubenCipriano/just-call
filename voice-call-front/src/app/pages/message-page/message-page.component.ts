import { Component } from "@angular/core";
import { ActivatedRoute } from "@angular/router";

@Component({
    selector: 'message-page',
    templateUrl: 'message-page.component.html',
    styleUrls: ['message-page.component.scss']
})

export class MessagePageComponent {
    constructor(private route: ActivatedRoute) {}

    ngOnInit() {
        this.route.params.subscribe(params => {
            console.log(params['channel_name'])
        });
      }
}