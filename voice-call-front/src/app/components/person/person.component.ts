import { Component, Input } from "@angular/core";
import { Person } from "../../models/Person";

@Component({
    selector: 'person',
    templateUrl: 'person.component.html',
    styleUrls: ['person.component.scss']
})

export class PersonComponent {
    @Input() person!: Person
}